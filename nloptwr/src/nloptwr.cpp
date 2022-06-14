
#include <nlopt.hpp>
#include "nloptwr/nloptwr.h"

#include "nloptwr/nloptwrparamfactory.h"
#include "nloptwr/nloptwrsstrat.h"
#include "nloptwr/lregx.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#include "nloptwr/omphelper.h"

#include <stdexcept>
#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>

using namespace std;

namespace nloptwr
{

// constructor
NLOptWrapper::NLOptWrapper (
  const oif::OptFknBase& oFktn, // number of non equality constraints
  unsigned int nThr
)
  :
  optFknBases(),
  nDim ( oFktn.getSizeOfX() ),
  mDim ( oFktn.getSizeOfNeConstraints() ),
  opt ( nullptr ),
  x ( oFktn.getXInitial() ),
  ub ( oFktn.getUpperBounds() ),
  lb ( oFktn.getLowerBounds() ),

  nThreads ( ( nThr>0 ) ? nThr : utl::OmpHelper::getNumProcs() ),
  dX ( nDim, 1.0E-8 )

{
  nlOptParamFactory=shared_ptr<NLOptWrParamFactory>(new NLOptWrParamFactory());

  initial_step.resize ( nDim );
  initialSubStep.resize ( nDim );
  minf=1.0e99;

  if ( x.size() !=nDim ) x.resize ( nDim );

  // if (xVec.size()!=nDim) xVec.resize(nDim);
  // if (cVec.size()!=mDim) cVec.resize(mDim);
  if ( fArgs.x.size() !=nDim ) fArgs.x.resize ( nDim );  // vector of arguments (nDim)
  if ( fArgs.c.size() !=mDim ) fArgs.c.resize ( mDim );  // vector of single constraints (mDim)
  if ( lb.size() !=nDim ) lb.resize ( nDim );
  if ( ub.size() !=nDim ) ub.resize ( nDim );
    
  // ========================================

  tolMConstraints.resize ( mDim );
  for ( size_t i=0; i<mDim; i++ )
    {
      tolMConstraints[i] = 0; // xTolAbs[0]*000.1; // TODO: use tol;constraintFactor
      fArgs.c[i] = 0.0;
    }

  // ========================================
    if ( xTolAbs.size() !=nDim ) xTolAbs.resize ( nDim );

    setSubFactors(0.1);
  
    setFTolAbs();
    setFTolRel();

    setXTolAbs();
    setXTolRel();
  
  // ========================================

  calculateInitialStep();

  if ( optFknBases.size() !=nThreads ) optFknBases.resize ( nThreads );
  for ( size_t i=0; i<optFknBases.size(); i++ ) optFknBases[i] = shared_ptr<oif::OptFknBase> ( oFktn.clone() );

  // ######### TODO
  useGradient=true;
  vectorStorage=10;
  useNewDerivMethod=false;
  derivRegrDim=3;
  
  derivRegrNoSteps=0;
  initLxReg();
  optTime=0.0;
  
};

NLOptWrapper::~NLOptWrapper()
{

}




void NLOptWrapper::setFTolAbs ( double val )
{
    fTolAbs=val;
}

    double NLOptWrapper::getFTolAbs () const {
     return fTolAbs;   
    }

double NLOptWrapper::getFTolAbsSubOpt () const {
 return (fTolAbs*subFTolAbsFactor);
}

void NLOptWrapper::setFTolRel ( double val )
{
    fTolRel=val;
}

    double NLOptWrapper::getFTolRel () const {
        return fTolRel;
    }

double NLOptWrapper::getFTolRelSubOpt () const {
    return (fTolRel*subFTolRelFactor);
}

void NLOptWrapper::setXTolAbs ( double val )
{
  for ( size_t i=0; i<nDim; i++ ) {
    xTolAbs[i] = val;
  }      
}


void NLOptWrapper::setXTolAbs ( const std::vector<double>& vals )
{
    // TODO: check vectors:  && i<vals.size()
  for ( size_t i=0; i<nDim; i++ ) {
      xTolAbs[i] = vals.at(i);
  }
}

const std::vector<double>& NLOptWrapper::getXTolAbs() const {
    return  xTolAbs; 
} 


std::vector<double> NLOptWrapper::getXTolAbsSubOpt() const {
    vector<double> xTolAbsSubOpt(nDim);
    for (size_t i=0; i<nDim; i++) { xTolAbsSubOpt[i]=xTolAbs[i]*subXTolAbsFactor; } 
    return xTolAbsSubOpt;
}


// ------------------------------------------------------------------------------------------------

// set realative tolerance
void NLOptWrapper::setXTolRel ( double val )
{
  xTolRel = val;
}

  double NLOptWrapper::getXTolRel () const {
      return xTolRel;
  }

// ------------------------------------------------------------------------------------------------

  double NLOptWrapper::getXTolRelSubOpt () const {
      return (xTolRel*subXtolRelFactor);
  }

size_t NLOptWrapper::getNThreads() const {
    return ((nThreads>0)? nThreads : 1);
}

void NLOptWrapper::setDx ( double val )
{
  for ( size_t i=0; i<nDim; i++ ) dX[i] = val;
}


void NLOptWrapper::setDx ( const std::vector<double>& vals )
{
  for ( size_t i=0; i<nDim && i<vals.size(); i++ ) dX[i] = vals[i];
}

void NLOptWrapper::setTolMConstraints ( double val )
{
  if ( dX.size() !=mDim ) dX.resize ( mDim );
  for ( size_t i=0; i<mDim; i++ ) dX[i] = val;
}


void NLOptWrapper::setTolMConstraints ( const std::vector<double>& vals )
{
  if ( dX.size() !=mDim ) dX.resize ( mDim );
  for ( size_t i=0; i<mDim && i<vals.size(); i++ ) dX[i] = vals[i];
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
     subFTolAbsFactor=(val<0.5)? val : 0.5;   
    }
    
    double NLOptWrapper::getSubFTolAbsFactor() const {
        return subFTolAbsFactor;
    }

/// reduction factor of relative fuction tolerance of subopt (lower than 1)
    void NLOptWrapper::setSubFTolRelFactor(double val)  {
     subFTolRelFactor=(val<0.5)? val : 0.5;   
    }
    

double NLOptWrapper::getSubFTolRelFactor() const {
 return subFTolRelFactor;   
}

/// eduction factor of relative arguments tolerance of subopt (lower than 1)
void NLOptWrapper::setSubXtolRelFactor(double val)  {
     subXtolRelFactor=(val<0.5)? val : 0.5;   
    }
    

double NLOptWrapper::getSubXtolRelFactor() const {
    return subXtolRelFactor;
}

/// eduction factor of absolute arguments tolerance of subopt (lower than 1)
void NLOptWrapper::setSubXTolAbsFactor(double val)  {
     subXTolAbsFactor=(val<0.5)? val : 0.5;   
    }
    

double NLOptWrapper::getSubXTolAbsFactor() const {
    return subXTolAbsFactor;
}

// ------------------------------------------------------------------------------------------------
    
void NLOptWrapper::calculateInitialStep ( double numberOfSteps )
{
  for ( size_t i=0; i<nDim; i++ )
    {
      initial_step[i] = ( ub[i]-lb[i] ) /numberOfSteps;
      initialSubStep[i] = initial_step[i]*0.1;
    }

}


bool NLOptWrapper::checkVectors() const
{
  bool result=true;

  if ( nDim!=lb.size() ) result=false;
  if ( nDim!=ub.size() ) result=false;
  if ( nDim!=xTolAbs.size() ) result=false;
  if ( nDim!=initial_step.size() ) result=false;
  if ( nDim!=initialSubStep.size() ) result=false;

  return result;
}

// get selecte algorithms of a given search getSearchStrategy
std::vector<NLOptWrAlgorithm> NLOptWrapper::getSelectedAlgorithms ( const NLOptWrSStrat& nloptWrSStrat ) const
{
  SEARCH_STRATEGY searchStrategy=nloptWrSStrat.getSearchStrategy();
  bool hasConstraints= ( mDim>0 );
  bool useAugLagBeforeMlsl=nloptWrSStrat.getUseAugLagBeforeMLSL();
  bool useGrad=nloptWrSStrat.getUseGradient();
  
  return ( *nlOptParamFactory ).getAlgorithm ( searchStrategy, hasConstraints, useGrad, useAugLagBeforeMlsl, nDim );
}

// ------------------------------------------------------------------------------------------------
// ================================================================================================

// optimization method
nlopt::result NLOptWrapper::optimize (
  double &fOpt,
  const NLOptWrSStrat& nloptWrSStrat,
  int maxTimeSec,
  int maxEvals
)
{
    if (optFknBases.empty()) {
      const string errMsg2 ( "NLOptWrapper::optimize : ERROR: optFknBases.empty() " );
      cerr << errMsg2 << endl;
      cerr.flush();
      throw runtime_error ( errMsg2 );
    } else {
      bool validateResult = optFknBases[0]->validate();
      if (!validateResult) {
        const string errMsg2 ( "NLOptWrapper::optimize : ERROR: optFknBases[0]->validate()==false : Not valid: lower_bound[i] <= x_initial[i] <= upper_bound[i] " );
        cerr << errMsg2 << endl;
        cerr.flush();
        throw runtime_error ( errMsg2 );
      }
    }

  utl::OmpHelper ompH;
  ompH.setStart();
  
  if ( x.size() !=nDim )
    {
      const string errMsg2 ( "NLOptWrapper::optimize : ERROR: x.size() !=nDim " );
      cerr << errMsg2 << endl;
      cerr.flush();
      throw runtime_error ( errMsg2 );
    }
  if ( lb.size() !=nDim )
    {
      const string errMsg2 ( "NLOptWrapper::optimize : ERROR: lb.size()!=nDim " );
      cerr << errMsg2 << endl;
      cerr.flush();
      throw runtime_error ( errMsg2 );
    }
  if ( ub.size() !=nDim )
    {
      const string errMsg2 ( "NLOptWrapper::optimize : ERROR: ub.size()!=nDim " );
      cerr << errMsg2 << endl;
      cerr.flush();
      throw runtime_error ( errMsg2 );
    }

  tolMConstraints.resize ( mDim ); // TODO

  // distance for differential
  if ( dX.size() !=nDim )
    {
      dX.resize ( nDim );
      for ( size_t i=0; i<nDim; i++ )
        {
          // TODO: Initalization in constructor
          dX[i]=0.00001;
        }
    }

  if ( fArgs.x.size() !=nDim ) fArgs.x.resize ( nDim );  // vector of arguments (nDim)
  if ( fArgs.c.size() !=mDim ) fArgs.c.resize ( mDim );  // vector of single constraints (mDim)

  fArgs1.resize ( nThreads );
  fArgs2.resize ( nThreads );
  for ( size_t i=0; i<nThreads; i++ )
    {
      fArgs1[i].x.resize ( nDim ); // vector of arguments (nDim)
      fArgs1[i].c.resize ( mDim ); // vector of single constraints (mDim)
      fArgs1[i].x = x;

      fArgs2[i].x.resize ( nDim ); // vector arguments (nDim)
      fArgs2[i].x = x;
      fArgs2[i].c.resize ( mDim ); // vector of single constraints (mDim)
    }

  // gradient of function (nDim)
  fGrad.resize ( nDim );

  // gradient of constraints (mDim*nDim)
  cGrad.resize ( mDim );
  for ( size_t i=0; i<mDim; i++ ) cGrad[i].resize ( nDim );

  // =========================================

  if ( !checkVectors() )
    {
      cerr << "Vectors are not initialized completely!" << endl;
      return nlopt::FAILURE;
    }

  // if ( !opt ) opt=nullptr;

  // compare the size of all vectors:
  if ( !checkVectors() ) throw std::invalid_argument ( "Different vector sizes" );


  // =====================================================
  // BEGIN(NEW implementation)
  // =====================================================

  std::vector<NLOptWrAlgorithm> algs = getSelectedAlgorithms ( nloptWrSStrat );
  if ( algs.empty() )
    {
      cerr << "ERROR: There are no proper algoritms found." << endl;
      throw runtime_error ( "There are no proper algoritms found." );
    }

  // for ( size_t j=0; j<algs.size(); j++ ) cout << "NLOptWrapper::optimize : algs[" << j<< "] = '" << algs[j].getName() << "'" << endl;
  // NEU
  for ( size_t j=0; j<algs.size(); j++ ) {
    useGradient=algs[j].getUseGradient();
  }

  const NLOptWrAlgorithm& alg1 = algs.at ( 0 );

  // nlopt_opt sub_opt=NULL;
  opt = shared_ptr<nlopt::opt>(new nlopt::opt ( alg1.getAlgorithmEnum(), nDim ) ); // algorithm and dimensionality

  if ( maxTimeSec>0 ) opt->set_maxtime ( maxTimeSec );
  
  if ( maxEvals>0 ) opt->set_maxeval ( maxEvals );

  // set bounds
  opt->set_upper_bounds ( ub );
  opt->set_lower_bounds ( lb );

  // set min objective
  opt->set_min_objective (
    [] ( unsigned int n, const double *x, double *grad, void *my_func_data )->double
  {
    return reinterpret_cast<NLOptWrapper*> ( my_func_data )->f ( n, x, grad );
  },
  this // ORIGINAL &data
  );

  opt->set_default_initial_step ( initial_step );

  if ( alg1.isSetPopulation() ) opt->set_population ( ( nDim+mDim ) *200 );

  if ( alg1.isVectorStorage() ) opt->set_vector_storage ( vectorStorage );

  if ( !alg1.isMLSL() )
    {
      // add an inequality m(ulti-)constraint
      opt->add_inequality_mconstraint (
        [] (
          unsigned int m,  // number of constraints
          double *result,  // vector of single contraint function result values
          unsigned int n,  // number of optimization parameters
          const double* x, // optimization parameters
          double *grad,
          void* f_data
        )
      {
        reinterpret_cast<NLOptWrapper*> ( f_data )
        ->multi_constraint (
          m,  // number of constraints
          result,  // vector of single contraint function result values
          n,  // number of optimization parameters
          x, // optimization parameters
          grad // gradient of constraints
        );
      },
      this, // ORIGINAL: &data,
      tolMConstraints
      );
    }

    // set tolerances of 1st algoritm
    opt->set_xtol_abs(getXTolAbs());
    opt->set_xtol_rel(getXTolRel());

    opt->set_ftol_abs(getFTolAbs());
    opt->set_ftol_rel(getFTolRel());

  // ==================================================
  // Do it only if there are constraints!
  if ( algs.size() >=2 )
    {
      const NLOptWrAlgorithm& alg2 = algs.at ( 1 );

      nlopt::opt sub_opt ( alg2.getAlgorithmEnum(), nDim );

      if (maxTimeSec>0) { sub_opt.set_maxtime(maxTimeSec); }
      if (maxEvals>0)   { sub_opt.set_maxeval(maxEvals); }

      // set bounds
      sub_opt.set_upper_bounds(ub);
      sub_opt.set_lower_bounds(lb);

      // set min objective
      sub_opt.set_min_objective(
      [](unsigned int n, const double *x, double *grad, void *my_func_data)->double {
         return reinterpret_cast<NLOptWrapper*>(my_func_data)->f(n, x, grad);
      },
      this // ORIGINAL &data
      );

    sub_opt.set_default_initial_step(initial_step);

    // TODO: set tolerances of 2nd algoritm

    sub_opt.set_xtol_abs(getXTolAbsSubOpt());
    sub_opt.set_xtol_rel(getXTolRelSubOpt());
    
    sub_opt.set_ftol_rel(getFTolRelSubOpt());
    sub_opt.set_ftol_abs(getFTolAbsSubOpt());

      if ( alg2.isSetPopulation() ) sub_opt.set_population ( ( nDim+mDim ) *200 );

      if ( alg2.isVectorStorage() ) sub_opt.set_vector_storage ( vectorStorage );

      sub_opt.set_default_initial_step ( initialSubStep );

      // NOTE: possible constraints in case of MLSL!!!!
      if ( !alg1.isAugLag() && alg2.canHandleNonEqualConstraints() )
        {
          // cout << "NLOptWrapper::optimize : (!alg1.isAugLag() && alg2.canHandleNonEqualConstraints()) " << endl;
          cout.flush();

          // add an inequality m(ulti-)constraint
          sub_opt.add_inequality_mconstraint (
            [] (
              unsigned int m,  // number of constraints
              double *result,  // vector of single contraint function result values
              unsigned int n,  // number of optimization parameters
              const double* x, // optimization parameters
              double *grad,
              void* f_data
            )
          {
            reinterpret_cast<NLOptWrapper*> ( f_data )
            ->multi_constraint (
              m,  // number of constraints
              result,  // vector of single contraint function result values
              n,  // number of optimization parameters
              x, // optimization parameters
              grad // gradient of constraints
            );
          },
          this, // ORIGINAL: &data,
          tolMConstraints
          );

        }

      cout.flush();
      opt->set_local_optimizer ( sub_opt );

    }

  // =====================================================
  // END(NEW implementation)
  // =====================================================

  cout.flush();

  // optimization
  nlopt::result opt_stat = opt->optimize ( x, minf );

  fOpt=minf;
  
  ompH.setEnd();
  optTime=ompH.getDuration();

  return opt_stat;
}

double NLOptWrapper::getFOpt()
{
  return minf;
}


const vector<double>& NLOptWrapper::getX() const
{
  return x;
};

// static
const char* NLOptWrapper::getStringOfResult ( nlopt::result r )
{
  // return ::nlopt_result_to_string(static_cast<nlopt_result>(r));
  switch ( r )
    {
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

bool NLOptWrapper::hasPreferedAlgorithms() const
{
  return ( *nlOptParamFactory ).hasPreferedAlgorithms();
}

void NLOptWrapper::deletePreferedAlgorithms()
{
  ( *nlOptParamFactory ).deletePreferedAlgorithms();
}

void NLOptWrapper::setPreferedAlgorithm ( nlopt::algorithm algx, int xDimMin, int xDimMax)
{
  ( *nlOptParamFactory ).setPreferedAlgorithm (algx, xDimMin, xDimMax);
}

void NLOptWrapper::setPreferedAlgorithms ( const std::vector<nlopt::algorithm>& algxs )
{
  ( *nlOptParamFactory ).setPreferedAlgorithms ( algxs );
}

const std::vector<NLOptWrAlgorithm>& NLOptWrapper::getPreferedAlgorithms() const
{
  return ( *nlOptParamFactory ).getPreferedAlgorithms();
}

void NLOptWrapper::setVectorStorage ( unsigned int val )
{
  vectorStorage=val;
}

unsigned int NLOptWrapper::getVectorStorage() const
{
  return vectorStorage;
}

// ===================================================================
// ===================================================================


bool NLOptWrapper::checkXVec ( unsigned int n, const double* x1 ) const
{
  bool rc = ( fArgs.x.size() ==n );
  if ( rc )
    {
      for ( size_t i=0; rc&& ( i<n ); ++i )
        {
          rc= ( fArgs.x[i] == x1[i] );
        }
    }

  return rc;
}

bool NLOptWrapper::setXVec ( unsigned int n, const double* x1 )
{
  bool rc=!checkXVec ( n, x1 );
  if ( rc )
    {
      if ( fArgs.x.size() !=n )
        {
          fArgs.x.resize ( n );
        }
      for ( size_t i=0; i<n; ++i )
        {
          fArgs.x[i] = x1[i];
        }
    }
  return rc;
}

// ===================================================================
// ===================================================================


bool NLOptWrapper::getNewDerivMethod() const {
    return useNewDerivMethod;
}
void NLOptWrapper::setNewDerivMethod(bool val) {
    useNewDerivMethod=val;
}


std::size_t NLOptWrapper::getDerivRegrDim() const {
    return derivRegrDim;
}

void NLOptWrapper::setDerivRegrDim(size_t d) {
    for (size_t i=0; i<nThreads; i++) {
        lregxVec[i]->resize(d);
    }
    derivRegrDim=d;
}

std::size_t NLOptWrapper::getDerivRegrNoSteps() const {
    return derivRegrNoSteps;
}


void NLOptWrapper::setDerivRegrNoSteps(std::size_t val) {
    if (val < 2*derivRegrDim+1) val=2*derivRegrDim+1;
    
    derivRegrNoSteps=val;
}

double NLOptWrapper::getOptTime() const {
 return optTime;   
}

// TODO
void NLOptWrapper::initLxReg() {
  lregxVec.resize(nThreads);
  lregxVecC.resize(nThreads);
  
  if (derivRegrNoSteps<=0) derivRegrNoSteps=2*derivRegrDim;
  
  for ( size_t i=0; i<nThreads; i++ ) {
      lregxVec[i]= std::shared_ptr<utilx::LRegX>(new utilx::LRegX(derivRegrDim));
      lregxVecC[i].resize(mDim);
      for ( size_t j=0; j<mDim; j++ ) {
          lregxVecC[i][j]= std::shared_ptr<utilx::LRegX>(new utilx::LRegX(derivRegrDim));
      }
  }
}


// ===================================================================
// ===================================================================

double NLOptWrapper::f ( unsigned n, const double *x1, double *fGradVal )
{
  bool useGradient = ( fGradVal!=nullptr ) ? true : false;

  // function call
  calcFktnConstrAndDeriv ( n, x1, useGradient );

  if ( useGradient )
    {
      for ( unsigned int i=0; i<n; i++ )
        {
          fGradVal[i] = fGrad[i];
        }
    }

  return fArgs.f;
}

// ===================================================================
// ===================================================================

void NLOptWrapper::multi_constraint (
  unsigned int m,  // number of constraints
  double *c,  // vector of single contraint function result values
  unsigned int n,  // number of optimization parameters
  const double* x, // optimization parameters
  double *cGradc  // gradient of constraints
)
{

  bool useGradient = ( cGradc!=nullptr ) ? true : false;

  // function call
  calcFktnConstrAndDeriv ( n, x, useGradient );

  for ( unsigned int k=0; k<m; k++ )
    {
      c[k] = fArgs.c[k];
      if ( useGradient ) 
        {
          for ( unsigned int i=0; i<n; i++ )
            {
              cGradc[k*n+i] = cGrad[k][i];
            }
        }
    }
}


// ===================================================================
// ===================================================================

// virtual
void NLOptWrapper::calcFktnConstrAndDeriv ( unsigned n, const double *x1, bool useGrad ) {
    if (useNewDerivMethod) {
        calcFktnConstrAndDerivNew (n, x1, useGrad );
    } else {
        calcFktnConstrAndDerivOld (n, x1, useGrad );
    }
    
}

// ===================================================================
// ===================================================================

// virtual
void NLOptWrapper::calcFktnConstrAndDerivOld ( unsigned n, const double *x1, bool useGrad )
{

  if ( setXVec ( n, x1 ) )
    {
      if ( useGrad || useGradient )
      // if ( useGrad && useGradient )
        {
          for ( size_t j=0; j<nThreads; j++ )
            {
              fArgs1[j].x=fArgs.x;
              fArgs2[j].x=fArgs.x;
            }

          #pragma omp parallel shared(optFknBases, fArgs, fArgs1, fArgs2, nDim, mDim, fGrad ) num_threads(nThreads)
          {
            #pragma omp for schedule(static)
            for ( size_t i=0; i<nDim; i++ )
              {
                size_t j=utl::OmpHelper::getThreadNum();

                double dx = dX[i];
                double half_dx = 0.5/dx;

                double xOld=fArgs1[j].x[i];
                fArgs2[j].x[i]= ( xOld+dx );
                fArgs1[j].x[i]= ( xOld-dx );

                // function calls
                fArgs2[j].f = optFknBases[j]->optFktn ( fArgs2[j].x, fArgs2[j].c );
                fArgs1[j].f = optFknBases[j]->optFktn ( fArgs1[j].x, fArgs1[j].c );


                fGrad[i] = ( fArgs2[j].f - fArgs1[j].f ) *half_dx;

                for ( size_t k=0; k<mDim; k++ )
                  {
                    cGrad[k][i] = ( fArgs2[j].c[k] - fArgs1[j].c[k] ) *half_dx;
                  }

                fArgs2[j].x[i]=xOld;
                fArgs1[j].x[i]=xOld;
              }

          }

        } // useGradient

      // 2.)   call fOld=optF(...)
      fArgs.f = optFknBases[0]->optFktn ( fArgs.x, fArgs.c );

    }
  else
    {
      // cout << "##### fArg2Deriv : reuse" << endl;
    }

}


// ===================================================================
// ===================================================================

// virtual
void NLOptWrapper::calcFktnConstrAndDerivNew ( unsigned n, const double *x1, bool useGrad )
{
  if ( setXVec ( n, x1 ) )
    {
        initLxReg();
    
      bool graD = ( useGrad || useGradient );

      // useGradient
          for ( size_t j=0; j<nThreads; j++ ) {
              fArgs1[j].x=fArgs.x;
            }

      double sumF=0.0;  
          size_t sum1=0;
          
          #pragma omp parallel shared(optFknBases, fArgs, fArgs1, fArgs2, nDim, mDim, fGrad, graD, sumF, sum1) num_threads(nThreads)
          {
            #pragma omp for schedule(static)
            for ( size_t i=0; i<nDim; i++ )
              {
                size_t j=utl::OmpHelper::getThreadNum();

                double dxFactor = dX[i]*3.0/static_cast<double>(derivRegrNoSteps);
                
                double xOld=fArgs1[j].x[i];
                double half_dx = 0.5/dX[i];
                
                // reset all lxreg counters:
                (*lregxVec[j]).reset();
                
                for (size_t k=0; k<mDim; k++) lregxVecC[j][k]->reset();
                
                for (int l=0-static_cast<int>(derivRegrNoSteps); l<=static_cast<int>(derivRegrNoSteps); l++) {
                    double dx=dxFactor*l;
                    
                    fArgs1[j].x[i]= ( xOld+dx );
                    
                    // function calls
                    fArgs1[j].f = optFknBases[j]->optFktn ( fArgs1[j].x, fArgs1[j].c );

                    // consider this data point
                    lregxVec[j]->add(dx, fArgs1[j].f);

                    for ( size_t k=0; k<mDim; k++ )  lregxVecC[j][k]->add(dx, fArgs1[j].c[k]);

                }
                
                std::vector<double> coeff;
                lregxVec[j]->calc(coeff);
                if (graD) fGrad[i]=coeff[1];
                
                #pragma omp atomic
                sumF+=coeff[0];
                
                #pragma omp atomic
                sum1+=1;
                
                for ( size_t k=0; k<mDim; k++ )
                {
                    lregxVecC[j][k]->calc(coeff);
                    if (graD) cGrad[k][i]=coeff[1];
                    fArgs.c[k]=coeff[0];
                }

                fArgs1[j].x[i]=xOld;
                
              }
          }
          
    
        // 2.)   call fOld=optF(...)
        fArgs.f = sumF/static_cast<double>(sum1);
    }
  else
    {
      // cout << "NLOptWrapper::calcFktnConstrAndDerivNew : fArg2Deriv : reuse" << endl;
    }

}


// ===================================================================
// ===================================================================

}

