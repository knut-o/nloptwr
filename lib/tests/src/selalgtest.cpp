
#include "nloptwrtest/selalgtest.h"
#include "nloptwr/nloptwra.h"

#include "nloptwr/nloptwralgorithm.h"

#include <cctype>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <tuple>

using namespace std;

namespace opttest {
/**
 * This function print the list of function names as text
 */
std::string pAlgo2(const vector<nloptwr::NLOptWrAlgorithm> &a) {
  stringstream ss;

  ss << "{";
  for (size_t i = 0; i < a.size(); i++) {
    ss << ((i > 0) ? ", " : " ");
    ss << "nlopt::" << a.at(i).getName();
  }
  ss << " }";

  return ss.str();
}

/**
 * This function print the list of function enums as text
 */
std::string pAlgo(const vector<nloptwr::NLOptWrAlgorithm> &a) {
  stringstream ss;

  ss << "{";
  for (size_t i = 0; i < a.size(); i++) {
    ss << ((i > 0) ? ", " : " ");
    ss << a.at(i).getAlgorithmEnum();
  }
  ss << " }";

  return ss.str();
}

/**
 * This function print the list of function positions as text
 */
std::string pAlgo(const vector<nlopt::algorithm> &a) {
  stringstream ss;

  ss << "{";
  for (size_t i = 0; i < a.size(); i++) {
    ss << ((i > 0) ? ", " : " ");
    ss << a.at(i);
  }
  ss << " }";

  return ss.str();
}

/**
 * Vector comparison
 */
bool isEqual(const vector<nlopt::algorithm> &a,
             const vector<nlopt::algorithm> &b) {
  bool rc = (a.size() == b.size());
  if (rc) {
    for (size_t i = 0; rc && i < a.size(); i++) {
      if (rc) {
        rc = (a.at(i) == b.at(i));
      }
    }
  }
  return rc;
}

/**
 * Vector comparison
 */
bool isEqual(const vector<nlopt::algorithm> &a,
             const vector<nloptwr::NLOptWrAlgorithm> &b) {
  bool rc = true;

  vector<nlopt::algorithm> b2;
  if (b2.capacity() < b.size())
    b2.reserve(b.size());
  for (size_t i = 0; rc && i < b.size(); i++) {
    b2.push_back(b.at(i).getAlgorithmEnum());
  }

  isEqual(a, b2);

  return rc;
}

// =============================================================================
// =============================================================================

/**
 * This function generates a list of algorithms
 * dependent of the search strategi and dimension
 * a source code text (to stdout)
 * @param nlOptParamFactory parameter parameter factory
 * @param xDimVec number of free parameters
 *
 */
int selAlgTestGenerate(const nloptwr::NLOptWrParamFactory &nlOptParamFactory,
                       vector<size_t> &xDimVec) {
  int rc = 0;

  cout << "Test" << endl;

  // opttest::OProblem oproblem(4);
  nloptwr::NLOptWrParamFactory f;
  nloptwr::NLOptWrSearchAlgoParam p4pAlgx(nloptwr::SSTRAT::G, false, false);
  f.deletePreferedAlgorithms();

  std::vector<nloptwr::SSTRAT> searchStrats{
      nloptwr::SSTRAT::GM, nloptwr::SSTRAT::LM, nloptwr::SSTRAT::N,
      nloptwr::SSTRAT::L,  nloptwr::SSTRAT::G,  nloptwr::SSTRAT::R};

  bool hasEqConstr = true;
  bool hasNeConstr = true;
  bool useGrad = true;

  // preferred algortitms provide different results:
  std::vector<nloptwr::NLOptWrAlgorithm> prefAlgs = f.getPreferedAlgorithms();
  for (size_t i = 0; i < prefAlgs.size(); i++)
    cout << "  prefAlgs[i=" << i << "] : " << prefAlgs.at(i).toString() << endl;

  cout << endl;

  cout << "std::vector<std::tuple<nloptwr::NLOptWrSearchAlgoParamD, "
          "vector<nlopt::algorithm> > > test0"
       << endl
       << " { " << endl
       << "   // { nloptwr::NLOptWrSearchAlgoParamD( SearchStrat, eqConstr, "
          "neConstr,  Grad, dim  )"
       << "  = { Algorithm, ... } }," << endl;

  for (size_t k = 0; k < xDimVec.size(); k++) {
    size_t dim = xDimVec.at(k);
    bool foundR = false;
    for (int is = 0; is < searchStrats.size() && !foundR; is++) {
      if (nloptwr::SSTRAT::N == searchStrats.at(is))
        continue;
      for (size_t cgs = 0; cgs < 8 && !foundR; cgs++) {
        useGrad = ((cgs & 1) != 0);
        hasEqConstr = ((cgs & 2) != 0);
        hasNeConstr = ((cgs & 4) != 0);

        nloptwr::SSTRAT searchStrat = searchStrats[is];

        if ((searchStrat == nloptwr::SSTRAT::R) && useGrad)
          continue;

        nloptwr::NLOptWrSearchAlgoParamD searchAlgoParamD(
            searchStrat, hasEqConstr, hasNeConstr, useGrad, dim);
        vector<nloptwr::NLOptWrAlgorithm> selectedAlgs =
            nlOptParamFactory.getAlgorithm(searchAlgoParamD);

        cout << " { "
             << "  " << searchAlgoParamD.toString2() << ", "
             << pAlgo2(selectedAlgs) << " }," << endl;
      }
    }
  }
  cout << " }; " << endl;
  return rc;
}

// =============================================================================

/**
 * comparison for regression test
 * @param nlOptParamFactory parameter factory
 * @param test0 solution to be compared
 * @return result: 0 is OK, otherwise is failed
 */
int selAlgTestCmp(
    const nloptwr::NLOptWrParamFactory &nlOptParamFactory,
    const std::vector<std::tuple<nloptwr::NLOptWrSearchAlgoParamD,
                                 vector<nlopt::algorithm>>> &test0) {
  int rc = 0;

  bool rcTmp = true;
  for (size_t i = 0; i < test0.size(); i++) {
    const std::tuple<nloptwr::NLOptWrSearchAlgoParamD, vector<nlopt::algorithm>>
        &ttuple = test0.at(i);
    const nloptwr::NLOptWrSearchAlgoParamD &searchAlgoParam3 = get<0>(ttuple);
    const vector<nlopt::algorithm> &eResult = get<1>(ttuple);

    vector<nloptwr::NLOptWrAlgorithm> tResult =
        nlOptParamFactory.getAlgorithm(searchAlgoParam3);

    rcTmp = isEqual(eResult, tResult);

    if (!rcTmp) {
      ++rc;
      cerr << "nlOptParamFactory.getAlgorithm(" << searchAlgoParam3.toString()
           << "), " << pAlgo(tResult) << " != " << pAlgo(eResult) << endl;
    }
  }

  return rc;
}

// =============================================================================

int selAlgTestCompare() {
  int rc = 0;
  nloptwr::NLOptWrParamFactory nlOptParamFactory;

  std::vector<
      std::tuple<nloptwr::NLOptWrSearchAlgoParamD, vector<nlopt::algorithm>>>
      test0{
          // { nloptwr::NLOptWrSearchAlgoParamD( SearchStrat, eqConstr,
          // neConstr,  Grad, dim  )  = { Algorithm, ... } },
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, false,
                                            false, 5),
           {nlopt::GN_DIRECT}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, false,
                                            true, 5),
           {nlopt::GD_STOGO}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, false,
                                            false, 5),
           {nlopt::LN_AUGLAG_EQ, nlopt::GN_DIRECT}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, false,
                                            true, 5),
           {nlopt::LD_AUGLAG_EQ, nlopt::GD_STOGO}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, true,
                                            false, 5),
           {nlopt::LN_AUGLAG, nlopt::GN_DIRECT}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, true,
                                            true, 5),
           {nlopt::LD_AUGLAG, nlopt::GD_STOGO}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, true,
                                            false, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, true,
                                            true, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, false,
                                            false, 5),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, false,
                                            true, 5),
           {nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, false,
                                            false, 5),
           {nlopt::LN_AUGLAG_EQ, nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, false,
                                            true, 5),
           {nlopt::LD_AUGLAG_EQ, nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, true,
                                            false, 5),
           {nlopt::LN_AUGLAG, nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, true,
                                            true, 5),
           {nlopt::LD_AUGLAG, nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, true,
                                            false, 5),
           {nlopt::LN_AUGLAG_EQ, nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, true,
                                            true, 5),
           {nlopt::LD_AUGLAG_EQ, nlopt::LD_MMA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, false,
                                            false, 5),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, false,
                                            true, 5),
           {nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, false,
                                            false, 5),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, false,
                                            true, 5),
           {nlopt::LD_SLSQP}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, true,
                                            false, 5),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, true,
                                            true, 5),
           {nlopt::LD_MMA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, true,
                                            false, 5),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, true,
                                            true, 5),
           {nlopt::LD_SLSQP}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, false,
                                            false, 5),
           {nlopt::GN_DIRECT}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, false,
                                            true, 5),
           {nlopt::GD_STOGO}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, false,
                                            false, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, false,
                                            true, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, true,
                                            false, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, true,
                                            true, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, true,
                                            false, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, true,
                                            true, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, false, false,
                                            false, 5),
           {nlopt::GN_CRS2_LM}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, true, false,
                                            false, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, false, true,
                                            false, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, true, true,
                                            false, 5),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, false,
                                            false, 15),
           {nlopt::GN_DIRECT}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, false,
                                            true, 15),
           {nlopt::GD_STOGO}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, false,
                                            false, 15),
           {nlopt::LN_AUGLAG_EQ, nlopt::GN_DIRECT}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, false,
                                            true, 15),
           {nlopt::LD_AUGLAG_EQ, nlopt::GD_STOGO}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, true,
                                            false, 15),
           {nlopt::LN_AUGLAG, nlopt::GN_DIRECT}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, true,
                                            true, 15),
           {nlopt::LD_AUGLAG, nlopt::GD_STOGO}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, true,
                                            false, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, true,
                                            true, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, false,
                                            false, 15),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, false,
                                            true, 15),
           {nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, false,
                                            false, 15),
           {nlopt::LN_AUGLAG_EQ, nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, false,
                                            true, 15),
           {nlopt::LD_AUGLAG_EQ, nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, true,
                                            false, 15),
           {nlopt::LN_AUGLAG, nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, true,
                                            true, 15),
           {nlopt::LD_AUGLAG, nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, true,
                                            false, 15),
           {nlopt::LN_AUGLAG_EQ, nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, true,
                                            true, 15),
           {nlopt::LD_AUGLAG_EQ, nlopt::LD_MMA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, false,
                                            false, 15),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, false,
                                            true, 15),
           {nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, false,
                                            false, 15),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, false,
                                            true, 15),
           {nlopt::LD_SLSQP}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, true,
                                            false, 15),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, true,
                                            true, 15),
           {nlopt::LD_MMA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, true,
                                            false, 15),
           {nlopt::LN_COBYLA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, true,
                                            true, 15),
           {nlopt::LD_SLSQP}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, false,
                                            false, 15),
           {nlopt::GN_DIRECT}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, false,
                                            true, 15),
           {nlopt::GD_STOGO}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, false,
                                            false, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, false,
                                            true, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, true,
                                            false, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, true,
                                            true, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, true,
                                            false, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, true,
                                            true, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, false, false,
                                            false, 15),
           {nlopt::GN_CRS2_LM}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, true, false,
                                            false, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, false, true,
                                            false, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, true, true,
                                            false, 15),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, false,
                                            false, 150),
           {nlopt::GN_DIRECT_L_RAND_NOSCAL}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, false,
                                            true, 150),
           {nlopt::GN_CRS2_LM}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, false,
                                            false, 150),
           {nlopt::LN_AUGLAG_EQ, nlopt::GN_DIRECT_L_RAND_NOSCAL}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, false,
                                            true, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, true,
                                            false, 150),
           {nlopt::LN_AUGLAG, nlopt::GN_DIRECT_L_RAND_NOSCAL}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, false, true,
                                            true, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, true,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::GM, true, true,
                                            true, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, false,
                                            false, 150),
           {nlopt::GN_CRS2_LM}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, false,
                                            true, 150),
           {nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, false,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, false,
                                            true, 150),
           {nlopt::LD_AUGLAG_EQ, nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, true,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, false, true,
                                            true, 150),
           {nlopt::LD_AUGLAG, nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, true,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::LM, true, true,
                                            true, 150),
           {nlopt::LD_AUGLAG_EQ, nlopt::LD_MMA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, false,
                                            false, 150),
           {nlopt::GN_CRS2_LM}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, false,
                                            true, 150),
           {nlopt::LD_LBFGS}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, false,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, false,
                                            true, 150),
           {nlopt::LD_SLSQP}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, true,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, false, true,
                                            true, 150),
           {nlopt::LD_MMA}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, true,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::L, true, true,
                                            true, 150),
           {nlopt::LD_SLSQP}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, false,
                                            false, 150),
           {nlopt::GN_DIRECT_L_RAND_NOSCAL}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, false,
                                            true, 150),
           {nlopt::GN_CRS2_LM}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, false,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, false,
                                            true, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, true,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, false, true,
                                            true, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, true,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::G, true, true,
                                            true, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, false, false,
                                            false, 150),
           {nlopt::GN_CRS2_LM}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, true, false,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, false, true,
                                            false, 150),
           {nlopt::GN_ISRES}},
          {nloptwr::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT::R, true, true,
                                            false, 150),
           {nlopt::GN_ISRES}},
      };

  rc = selAlgTestCmp(nlOptParamFactory, test0);
  return rc;
}
} // namespace opttest

// =============================================================================
// =============================================================================

int main(int argc, const char *argv[]) {
  int rc = 0;

  // name of the program
  string prog;

  // dimension
  vector<size_t> xDim;

  // parameter factory
  nloptwr::NLOptWrParamFactory nlOptParamFactory;

  // flag for preferred algorithms
  bool usePreferedAlgoritms = false;

  // flag for output generation
  bool generateOutPut = false;

  for (int i = 0; i < argc; i++) {
    if (0 == i)
      prog = argv[i];
    if (i > 0) {
      string sizeStr = argv[i];
      if (!sizeStr.empty()) {
        if (!isdigit(sizeStr[0])) {
          cout << endl
               << "cout usage:" << endl
               << " " << argv[0] << " [dim1] [dim2] ... [dimN] " << endl
               << " # dim1, dim2 = dimensions of optimization problems" << endl
               << " # In case of no dimension the the results are validated."
               << endl
               << endl;

          exit(1);
        }
      }

      stringstream is(sizeStr);
      size_t xd = 0;
      is >> xd;
      if (xDim.size() >= xDim.capacity())
        xDim.reserve(2 * (xDim.size() + 1));
      xDim.push_back(xd);

      if (!generateOutPut)
        generateOutPut = true;
    }
  }

  if (usePreferedAlgoritms) {
    nlOptParamFactory.setPreferedAlgorithm(nlopt::GD_STOGO, 1, 15);
    nlOptParamFactory.setPreferedAlgorithm(nlopt::GN_AGS, 2, 5);
    nlOptParamFactory.setPreferedAlgorithm(nlopt::LN_SBPLX, 1, 15);
    nlOptParamFactory.setPreferedAlgorithm(nlopt::LD_TNEWTON_PRECOND_RESTART, 0,
                                           1000);
    nlOptParamFactory.setPreferedAlgorithm(nlopt::LD_SLSQP, 0, 10);
    nlOptParamFactory.setPreferedAlgorithm(nlopt::LD_MMA, 0, 500);
    nlOptParamFactory.setPreferedAlgorithm(nlopt::LN_NELDERMEAD, 0, 12);
  }

  cout << "Parameter generateOutPut=" << ((generateOutPut) ? "true" : "false")
       << endl;
  if (generateOutPut) {
    // generate output
    rc = opttest::selAlgTestGenerate(nlOptParamFactory, xDim);
  } else {
    // regression test
    rc = opttest::selAlgTestCompare();

    cout << "The test was " << ((rc == 0) ? "" : "not") << "successful. "
         << endl;
  }

  cout << endl << "rc=" << rc << endl << endl;

  return rc;
}
