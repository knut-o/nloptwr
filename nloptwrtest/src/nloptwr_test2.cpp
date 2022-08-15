
#include "nloptwr/nloptwra.h"

#include "nloptwrtest/griewank.h"
#include "nloptwrtest/odisplay.h"
#include "nloptwrtest/oproblem.h"
#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/vcompare.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

// extern int auglag_verbose;
// extern int ccsa_verbose;
// extern int mma_verbose;
// extern int testfuncs_verbose;

namespace opttest {

int opttest2(int dim, bool useGrad, nloptwr::SSTRAT sStrat = nloptwr::SSTRAT::L,
             int maxTimeSec = 400) {
  // return code
  int rc = 0;

  // number of threads ("0" means autodetect)
  int nThr = 0; // TODO

  int maxEvals = 1000000000;

  // create the search strategy object
  nloptwr::NLOptWrSStrat sStrategy(sStrat, useGrad);

  cout << "sStrategy.getUseGradient(): " << sStrategy.getUseGradient() << endl;

  // utl::OmpHelper ompHelper;
  OProblem oProblem(dim);

  oif::OptFknBase &oTarget = oProblem;

  nloptwr::NLOptWrapper optWr(oTarget, nThr);

  // parallel evaluations need less time
  if (useGrad)
    maxTimeSec /= static_cast<int>(optWr.getNThreads());

  optWr.calculateInitialStep(1.3);

  // set population for stochastic optimization
  optWr.setPopulation(1000);

  optWr.setPreferedAlgorithm(nlopt::LD_MMA);
  optWr.setPreferedAlgorithm(nlopt::LD_CCSAQ);

  optWr.setDx(1e-10);
  optWr.setXTolAbs(1e-6);

  // start opptimization
  nlopt::result opt_stat = nlopt::result::FAILURE;

  try {
    opt_stat = optWr.optimize(sStrategy, maxTimeSec, maxEvals);
  } catch (runtime_error eRt) {
    cerr << "ERROR: runtime_error : " << eRt.what() << endl;
    cerr.flush();
  } catch (exception e) {
    cerr << "ERROR: exception : " << e.what() << endl;
    cerr.flush();
    throw exception(e);
  }

  // get the optimized value
  double minf = optWr.getLastOptimumValue();

  // ========================================================================

  // Display the solution:
  rc = opttest::display(optWr, sStrategy, opt_stat, minf);

  // expected solution:
  vector<double> solution(dim, 1.0);
  solution[0] = 1.0 / 3.0;
  solution[1] = 8.0 / 27.0;
  double fOptExpcted = 0.544331;

  // test
  bool isOk =
      opttest::fvcompare(minf, fOptExpcted, optWr.getX(), solution, 0.009);
  if (!isOk) {
    rc = 1;
    cerr << "The optimization has failed!" << endl;
  }

  return rc;

}; // main

} // namespace opttest

// =====================================================
// =====================================================

int main(int argc, char *argv[]) {
  int dim = 4;
  int maxTimeSec = 300;
  nloptwr::SSTRAT sStrat = nloptwr::SSTRAT::LM;
  bool useGrad = true;

  if (argc <= 2) {
    cout << endl
         << "cout usage:" << endl
         << " " << argv[0] << " [dim] [useGrad] [sStrat] [maxTimeSec] " << endl
         << " # dim=dimension of problem " << endl
         << " # grad=usage of gradients (1 means with gradients (default), "
            "otherwise gradients are not used) "
         << endl
         << " # sStrat: L=Local, LM=Local Meta., G=Global, GM=Global Meta; "
            "R=Random (default='"
         << getStrategyAsString(sStrat) << "')" << endl
         << " # maxTimeSec: max. time [s] (default=" << maxTimeSec << ") "
         << endl
         << endl;
  }

  for (int i = 1; i < argc; ++i) {
    if (i == 1)
      dim = atoi(argv[i]);
    if (i == 2)
      useGrad = (string(argv[i]) == "1");
    if (i == 3)
      sStrat = nloptwr::getStrategyFromString(argv[i]);
    if (i == 4)
      maxTimeSec = atoi(argv[i]);
  }

  if (dim < 2) {
    dim = 2;
  }

  cout << "### dim: " << dim << " " << endl;
  cout << "### useGrad: '" << useGrad << "' " << endl;
  cout << "### sStrat: '" << getStrategyAsString(sStrat) << "' " << endl;

  int rc = opttest::opttest2(dim, useGrad, sStrat, maxTimeSec);

  cout << endl;

  return rc;
}
