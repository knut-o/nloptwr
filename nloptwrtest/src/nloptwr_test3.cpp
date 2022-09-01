
#include "nloptwr/nloptwra.h"

#include "nloptwrtest/odisplay.h"
#include "nloptwrtest/oproblem.h"
#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/vcompare.h"

#include <vector>
using namespace std;

namespace opttest {

int opttest3() {
  int rc = 0;

  int dim = 3000;

  // number of threads ("0" means autodetect)
  int nThr = 0;

  // timeout of algorithm
  int maxTimeSec = 120;

  // maximal count of evaluations
  int maxEvals = 1000000000;

  // use gradient
  bool useGrad = true;

  // other parameter
  bool useAugLagBeforeMlsl = true;

  // =====================================================

  // a target function
  Griewank oGriewank(dim);

  // initial point is near the optimum
  oGriewank.initialize(-15.0, 15.0, 2.0);

  // it is used a local search (because of the speed)
  nloptwr::NLOptWrSStrat sStrategy(nloptwr::SSTRAT::L, useGrad);

  // =====================================================

  // get reference of the target function
  oif::OptFknBase &oTarget = oGriewank;

  // create the NLOptWrapper object
  nloptwr::NLOptWrapper optWr(oTarget, nThr);

  // parallel evaluations need less time
  if (useGrad)
    maxTimeSec /= static_cast<int>(optWr.getNThreads());

  nlopt::result opt_stat = nlopt::result::FAILURE;

  try {
    // start opptimization
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

  // =====================================================

  // display the result
  rc = opttest::display(optWr, sStrategy, opt_stat, minf);

  // check the results
  bool isOk = opttest::fvcompare(minf, 0.0, optWr.getX(), 0.0);
  if (!isOk) {
    rc = 1;
    cerr << "The 2nd optimization has failed!" << endl;
  }

  return rc;

}; // main

} // namespace opttest

// =====================================================

int main() {
  int rc = opttest::opttest3();

  return rc;
}
