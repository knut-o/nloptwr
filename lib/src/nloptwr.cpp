
#include <nlopt.hpp>

#include "nloptwr/nloptwr.h"
#include "nloptwr/nloptwrfcargs.h"
#include "nloptwr/nloptwrparamfactory.h"
#include "nloptwr/nloptwrsstrat.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#include "nloptwr/omphelper.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace std;

namespace nloptwr {

// constructor
NLOptWrapper::NLOptWrapper(
    const oif::OptFknBase &oFktn, // number of non equality constraints
    unsigned int nThr)
    : optFknBases(), nDim(static_cast<long int>(oFktn.getSizeOfX())),

      mEqDim(static_cast<long int>(oFktn.getSizeOfEqConstraints())),
      mNeDim(static_cast<long int>(oFktn.getSizeOfNeConstraints())),

      opt(nullptr), x(oFktn.getXInitial()), ub(oFktn.getUpperBounds()),
      lb(oFktn.getLowerBounds()),
      nThreads(static_cast<size_t>((nThr > 0) ? nThr
                                              : utl::OmpHelper::getNumProcs())),
      dX(nDim, 1.0E-8)

{
  nlOptParamFactory =
      shared_ptr<NLOptWrParamFactory>(new NLOptWrParamFactory());

  initial_step.resize(nDim);
  initialSubStep.resize(nDim);
  fOpt = 1.0e99;

  if (x.size() != nDim)
    x.resize(nDim);

  if (static_cast<long int>(lb.size()) != nDim)
    lb.resize(nDim);
  if (static_cast<long int>(ub.size()) != nDim)
    ub.resize(nDim);

  // ========================================
  if (static_cast<long int>(xTolAbs.size()) != nDim)
    xTolAbs.resize(nDim);

  // ========================================

  setSubFactors(0.1);

  setFTolAbs();
  setFTolRel();

  setXTolAbs();
  setXTolRel();

  setTolMConstraints();

  // ========================================

  calculateInitialStep();

  // ========================================

  if (optFknBases.size() != static_cast<size_t>(nThreads))
    optFknBases.resize(static_cast<size_t>(nThreads));
  for (size_t i = 0; i < optFknBases.size(); i++)
    optFknBases[i] = shared_ptr<oif::OptFknBase>(oFktn.clone());

  useGradient = true;
  vectorStorage = 10;
  useNewDerivMethod = false;
  derivRegrDim = 3;

  optTime = 0.0;

  searchMin = true;
  population = 0;
};

NLOptWrapper::~NLOptWrapper() {}

void NLOptWrapper::setFTolAbs(double val) { fTolAbs = val; }

double NLOptWrapper::getFTolAbs() const { return fTolAbs; }

double NLOptWrapper::getFTolAbsSubOpt() const {
  return (fTolAbs * subFTolAbsFactor);
}

void NLOptWrapper::setFTolRel(double val) { fTolRel = val; }

double NLOptWrapper::getFTolRel() const { return fTolRel; }

double NLOptWrapper::getFTolRelSubOpt() const {
  return (fTolRel * subFTolRelFactor);
}

void NLOptWrapper::setXTolAbs(double val) {
  for (long int i = 0; i < nDim; i++) {
    xTolAbs[i] = val;
  }
}

void NLOptWrapper::setXTolAbs(const std::vector<double> &vals) {
  // TODO: check vectors:  && i<vals.size()
  for (long int i = 0; i < nDim; i++) {
    xTolAbs[i] = vals.at(i);
  }
}

const std::vector<double> &NLOptWrapper::getXTolAbs() const { return xTolAbs; }

std::vector<double> NLOptWrapper::getXTolAbsSubOpt() const {
  vector<double> xTolAbsSubOpt(nDim);
  for (long int i = 0; i < nDim; i++) {
    xTolAbsSubOpt[i] = xTolAbs[i] * subXTolAbsFactor;
  }
  return xTolAbsSubOpt;
}

// ------------------------------------------------------------------------------------------------

// set realative tolerance
void NLOptWrapper::setXTolRel(double val) { xTolRel = val; }

double NLOptWrapper::getXTolRel() const { return xTolRel; }

// ------------------------------------------------------------------------------------------------

double NLOptWrapper::getXTolRelSubOpt() const {
  return (xTolRel * subXtolRelFactor);
}

size_t NLOptWrapper::getNThreads() const {
  return ((nThreads > 0) ? nThreads : 1);
}

void NLOptWrapper::setDx(double val) {
  for (long int i = 0; i < nDim; i++)
    dX[i] = val;
}

void NLOptWrapper::setDx(const std::vector<double> &vals) {
  for (long int i = 0; i < nDim && i < static_cast<long int>(vals.size()); i++)
    dX[i] = vals[i];
}

void NLOptWrapper::setTolMConstraints(double val) {
  setTolEqMConstraints(val);
  setTolNeMConstraints(val);
}

void NLOptWrapper::setTolEqMConstraints(double val) {
  if (static_cast<long int>(tolEqMConstraints.size()) != mEqDim)
    tolEqMConstraints.resize(mEqDim);
  for (size_t i = 0; i < mEqDim; i++)
    tolEqMConstraints[i] = val;
}

void NLOptWrapper::setTolNeMConstraints(double val) {
  if (static_cast<long int>(tolNeMConstraints.size()) != mNeDim)
    tolNeMConstraints.resize(mNeDim);
  for (size_t i = 0; i < mNeDim; i++)
    tolNeMConstraints[i] = val;
}

void NLOptWrapper::setTolEqMConstraints(const std::vector<double> &vals) {
  if (static_cast<long int>(tolEqMConstraints.size()) != mEqDim)
    throw runtime_error("NLOptWrapper::setTolEqMConstraints : "
                        "(tolEqMConstraints.size() != mEqDim)");
  for (size_t i = 0; i < mEqDim && i < vals.size(); i++)
    tolEqMConstraints[i] = vals[i];
}

void NLOptWrapper::setTolNeMConstraints(const std::vector<double> &vals) {
  if (static_cast<long int>(tolNeMConstraints.size()) != mNeDim)
    throw runtime_error("NLOptWrapper::setTolNeMConstraints : "
                        "(tolNeMConstraints.size() != mNeDim)");
  for (size_t i = 0; i < mNeDim; i++)
    tolNeMConstraints[i] = vals[i];
}

// ------------------------------------------------------------------------------------------------

void NLOptWrapper::setSubFactors() {
  setSubFTolAbsFactor();
  setSubFTolRelFactor();
  setSubXtolRelFactor();
  setSubXTolAbsFactor();
}

void NLOptWrapper::setSubFactors(double val) {
  setSubFTolAbsFactor(val);
  setSubFTolRelFactor(val);
  setSubXtolRelFactor(val);
  setSubXTolAbsFactor(val);
}

void NLOptWrapper::setSubFTolAbsFactor(double val) {
  subFTolAbsFactor = (val < 0.5) ? val : 0.5;
}

double NLOptWrapper::getSubFTolAbsFactor() const { return subFTolAbsFactor; }

/// reduction factor of relative function tolerance of subopt (lower than 1)
void NLOptWrapper::setSubFTolRelFactor(double val) {
  subFTolRelFactor = (val < 0.5) ? val : 0.5;
}

double NLOptWrapper::getSubFTolRelFactor() const { return subFTolRelFactor; }

/// eduction factor of relative arguments tolerance of subopt (lower than 1)
void NLOptWrapper::setSubXtolRelFactor(double val) {
  subXtolRelFactor = (val < 0.5) ? val : 0.5;
}

double NLOptWrapper::getSubXtolRelFactor() const { return subXtolRelFactor; }

/// eduction factor of absolute arguments tolerance of subopt (lower than 1)
void NLOptWrapper::setSubXTolAbsFactor(double val) {
  subXTolAbsFactor = (val < 0.5) ? val : 0.5;
}

double NLOptWrapper::getSubXTolAbsFactor() const { return subXTolAbsFactor; }

// ------------------------------------------------------------------------------------------------

void NLOptWrapper::calculateInitialStep(double numberOfSteps) {
  for (long int i = 0; i < nDim; i++) {
    initial_step[i] = (ub[i] - lb[i]) / numberOfSteps;
    initialSubStep[i] = initial_step[i] * 0.1;
  }
}

bool NLOptWrapper::checkVectors() const {
  bool result = true;

  if (nDim != static_cast<long int>(lb.size()))
    result = false;
  if (nDim != static_cast<long int>(ub.size()))
    result = false;
  if (nDim != static_cast<long int>(xTolAbs.size()))
    result = false;
  if (nDim != static_cast<long int>(initial_step.size()))
    result = false;
  if (nDim != static_cast<long int>(initialSubStep.size()))
    result = false;

  return result;
}

// get selected algorithms of a given search getSearchStrategy
std::vector<NLOptWrAlgorithm>
NLOptWrapper::getSelectedAlgorithms(const NLOptWrSStrat &nloptWrSStrat) const {
  SSTRAT searchStrategy(nloptWrSStrat.getSearchStrategy());
  bool hasConstraints = ((mEqDim > 0) || (mNeDim > 0));
  bool useGrad = nloptWrSStrat.getUseGradient();

  return (*nlOptParamFactory)
      .getAlgorithm(searchStrategy, hasConstraints, useGrad, nDim);
}

// ------------------------------------------------------------------------------------------------
void NLOptWrapper::setSearchMin(bool sMin) { searchMin = sMin; }

bool NLOptWrapper::getSearchMin() const { return searchMin; }

// TODO
double NLOptWrapper::getLastOptimumValue() const { return fOpt; }
// ------------------------------------------------------------------------------------------------
// ================================================================================================

// optimization method

// optimization method
nlopt::result NLOptWrapper::optimize(const NLOptWrSStrat &nloptWrSStrat,
                                     int maxTimeSec, int maxEvals) {
  fOpt =
      ((searchMin) ? numeric_limits<double>::max() : numeric_limits<double>::min());

  if (optFknBases.empty()) {
    const string errMsg2(
        "NLOptWrapper::optimize : ERROR: optFknBases.empty() ");
    cerr << errMsg2 << endl;
    cerr.flush();
    throw runtime_error(errMsg2);
  } else {
    bool validateResult = optFknBases[0]->validate();
    if (!validateResult) {
      const string errMsg2(
          "NLOptWrapper::optimize : ERROR: optFknBases[0]->validate()==false : "
          "Not valid: lower_bound[i] <= x_initial[i] <= upper_bound[i] ");
      cerr << errMsg2 << endl;
      cerr.flush();
      throw runtime_error(errMsg2);
    }
  }

  utl::OmpHelper ompH;
  ompH.setStart();

  if (static_cast<long int>(x.size()) != nDim) {
    const string errMsg2("NLOptWrapper::optimize : ERROR: x.size() !=nDim ");
    cerr << errMsg2 << endl;
    cerr.flush();
    throw runtime_error(errMsg2);
  }
  if (static_cast<long int>(lb.size()) != nDim) {
    const string errMsg2("NLOptWrapper::optimize : ERROR: lb.size()!=nDim ");
    cerr << errMsg2 << endl;
    cerr.flush();
    throw runtime_error(errMsg2);
  }
  if (static_cast<long int>(ub.size()) != nDim) {
    const string errMsg2("NLOptWrapper::optimize : ERROR: ub.size()!=nDim ");
    cerr << errMsg2 << endl;
    cerr.flush();
    throw runtime_error(errMsg2);
  }

  if (tolEqMConstraints.size() != mEqDim)
    setTolEqMConstraints();
  if (tolNeMConstraints.size() != mNeDim)
    setTolNeMConstraints();

  // distance for differential
  if (static_cast<long int>(dX.size()) != nDim) {
    dX.resize(nDim);
    for (long int i = 0; i < nDim; i++) {
      // TODO: Initialization in constructor
      dX[i] = 0.00001;
    }
  }

  // =========================================

  if (!checkVectors()) {
    cerr << "Vectors are not initialized completely!" << endl;
    return nlopt::FAILURE;
  }

  // compare the size of all vectors:
  if (!checkVectors())
    throw std::invalid_argument("Different vector sizes");

  // =====================================================
  // =====================================================

  // initialize calculation object
  nLOptWrFCArgs.init(optFknBases, dX);

  // =====================================================
  // =====================================================

  std::vector<NLOptWrAlgorithm> algs = getSelectedAlgorithms(nloptWrSStrat);
  if (algs.empty()) {
    cerr << "ERROR: There are no proper algorithms found." << endl;
    throw runtime_error("There are no proper algorithms found.");
  }

  // for ( size_t j=0; j<algs.size(); j++ ) cout << "NLOptWrapper::optimize :
  // algs[" << j<< "] = '" << algs[j].getName() << "'" << endl; NEW
  for (size_t j = 0; j < algs.size(); j++) {
    useGradient = algs[j].getUseGradient();
  }

  const NLOptWrAlgorithm &alg1 = algs.at(0);

  // nlopt_opt sub_opt=NULL;
  opt = shared_ptr<nlopt::opt>(new nlopt::opt(
      alg1.getAlgorithmEnum(), nDim)); // algorithm and dimensionality

  if (maxTimeSec > 0)
    opt->set_maxtime(maxTimeSec);

  if (maxEvals > 0)
    opt->set_maxeval(maxEvals);

  // set bounds
  opt->set_upper_bounds(ub);
  opt->set_lower_bounds(lb);

  // set min objective
  if (searchMin) {
    opt->set_min_objective(
        [](unsigned int n, const double *x, double *grad,
           void *my_func_data) -> double {

          return reinterpret_cast<NLOptWrapper *>(my_func_data)
              ->nLOptWrFCArgs.f(n, x, grad);
        },
        this // ORIGINAL &data
    );
  } else {
    opt->set_max_objective(
        [](unsigned int n, const double *x, double *grad,
           void *my_func_data) -> double {
          return reinterpret_cast<NLOptWrapper *>(my_func_data)
              ->nLOptWrFCArgs.f(n, x, grad);
        },
        this // ORIGINAL &data
    );
  }

  opt->set_default_initial_step(initial_step);

  // TODO:
  if (alg1.isSetPopulation())
    opt->set_population(population);

  if (alg1.isVectorStorage())
    opt->set_vector_storage(vectorStorage);

  if (!alg1.isMLSL()) {

    if (nLOptWrFCArgs.hasEqConstraints()) {
      // add an inequality m(ulti-)constraint
      opt->add_equality_mconstraint(
          [](unsigned int m, // number of constraints
             double
                 *result, // vector of single constraint function result values
             unsigned int n,   // number of optimization parameters
             const double *x1, // optimization parameters
             double *grad, void *f_data) {
            reinterpret_cast<NLOptWrapper *>(f_data)
                ->nLOptWrFCArgs.multiEqCconstraint(
                    m,      // number of constraints
                    result, // vector of single constraint function result
                            // values
                    n,      // number of optimization parameters
                    x1,     // optimization parameters
                    grad    // gradient of constraints
                );
          },
          this, // ORIGINAL: &data,
          tolEqMConstraints);
    }

    if (nLOptWrFCArgs.hasNeConstraints()) {
      // add an inequality m(ulti-)constraint
      opt->add_inequality_mconstraint(
          [](unsigned int m, // number of constraints
             double
                 *result, // vector of single constraint function result values
             unsigned int n,   // number of optimization parameters
             const double *x1, // optimization parameters
             double *grad, void *f_data) {
            reinterpret_cast<NLOptWrapper *>(f_data)
                ->nLOptWrFCArgs.multiNeCconstraint(
                    // multi_constraint(
                    m,      // number of constraints
                    result, // vector of single constraint function result
                            // values values
                    n,      // number of optimization parameters
                    x1,     // optimization parameters
                    grad    // gradient of constraints
                );
          },
          this, // ORIGINAL: &data,
          tolNeMConstraints);
    }
  }

  // set tolerances of 1st algorithm
  opt->set_xtol_abs(getXTolAbs());
  opt->set_xtol_rel(getXTolRel());

  opt->set_ftol_abs(getFTolAbs());
  opt->set_ftol_rel(getFTolRel());

  // ==================================================
  // Do it only if there are constraints!
  if (algs.size() >= 2) {
    const NLOptWrAlgorithm &alg2 = algs.at(1);

    nlopt::opt sub_opt(alg2.getAlgorithmEnum(), nDim);

    if (maxTimeSec > 0) {
      sub_opt.set_maxtime(maxTimeSec);
    }
    if (maxEvals > 0) {
      sub_opt.set_maxeval(maxEvals);
    }

    // set bounds
    sub_opt.set_upper_bounds(ub);
    sub_opt.set_lower_bounds(lb);
    sub_opt.set_default_initial_step(initial_step);

    sub_opt.set_xtol_abs(getXTolAbsSubOpt());
    sub_opt.set_xtol_rel(getXTolRelSubOpt());

    sub_opt.set_ftol_rel(getFTolRelSubOpt());
    sub_opt.set_ftol_abs(getFTolAbsSubOpt());

    if (alg2.isSetPopulation())
      sub_opt.set_population(population);

    if (alg2.isVectorStorage())
      sub_opt.set_vector_storage(vectorStorage);

    sub_opt.set_default_initial_step(initialSubStep);

    opt->set_local_optimizer(sub_opt);
  }

  // optimization
  nlopt::result opt_stat = opt->optimize(x, fOpt);

  ompH.setEnd();
  optTime = ompH.getDuration();

  return opt_stat;
}

double NLOptWrapper::getFOpt() { return fOpt; }

const vector<double> &NLOptWrapper::getX() const { return x; };

// static
const char *NLOptWrapper::getStringOfResult(nlopt::result r) {
  // return ::nlopt_result_to_string(static_cast<nlopt_result>(r));
  switch (r) {
  case nlopt::FAILURE:
    return "FAILURE";
  case nlopt::INVALID_ARGS:
    return "INVALID_ARGS";
  case nlopt::OUT_OF_MEMORY:
    return "OUT_OF_MEMORY";
  case nlopt::ROUNDOFF_LIMITED:
    return "ROUNDOFF_LIMITED";
  case nlopt::FORCED_STOP:
    return "FORCED_STOP";
  case nlopt::SUCCESS:
    return "SUCCESS";
  case nlopt::STOPVAL_REACHED:
    return "STOPVAL_REACHED";
  case nlopt::FTOL_REACHED:
    return "FTOL_REACHED";
  case nlopt::XTOL_REACHED:
    return "XTOL_REACHED";
  case nlopt::MAXEVAL_REACHED:
    return "MAXEVAL_REACHED";
  case nlopt::MAXTIME_REACHED:
    return "MAXTIME_REACHED";
  default:
    return "UNKNOWN RESULT";
  }
  return "UNKNOWN RESULT";
};

// ===================================================================
// ===================================================================

bool NLOptWrapper::hasPreferedAlgorithms() const {
  return (*nlOptParamFactory).hasPreferedAlgorithms();
}

void NLOptWrapper::deletePreferedAlgorithms() {
  (*nlOptParamFactory).deletePreferedAlgorithms();
}

void NLOptWrapper::setPreferedAlgorithm(nlopt::algorithm algx, int xDimMin,
                                        int xDimMax) {
  (*nlOptParamFactory).setPreferedAlgorithm(algx, xDimMin, xDimMax);
}

void NLOptWrapper::setPreferedAlgorithms(
    const std::vector<nlopt::algorithm> &algxs) {
  (*nlOptParamFactory).setPreferedAlgorithms(algxs);
}

const std::vector<NLOptWrAlgorithm> &
NLOptWrapper::getPreferedAlgorithms() const {
  return (*nlOptParamFactory).getPreferedAlgorithms();
}

unsigned int NLOptWrapper::getPopulation() const { return population; }

void NLOptWrapper::setPopulation(unsigned int val) {
  population = static_cast<unsigned int>(val);
}

void NLOptWrapper::setVectorStorage(unsigned int val) { vectorStorage = val; }

unsigned int NLOptWrapper::getVectorStorage() const { return vectorStorage; }

// ===================================================================
// ===================================================================

double NLOptWrapper::getOptTime() const { return optTime; }

// ===================================================================
// ===================================================================

} // namespace nloptwr
