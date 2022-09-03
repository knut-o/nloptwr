
#include "nloptwr/nloptwra.h"

#include "nloptwrtest/chainwithweights.h"
#include "nloptwrtest/griewank.h"
#include "nloptwrtest/odisplay.h"
#include "nloptwrtest/vcompare.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

namespace opttest {

int opttest6(int dim, bool useGrad, nloptwr::SSTRAT sStrat = nloptwr::SSTRAT::L,
             int maxTimeSec = 400) {
  // return code
  int rc = 0;

  // number of threads ("0" means autodetect)
  int nThr = 0;

  int maxEvals = 1000000000;

  double xN = static_cast<double>(dim);
  double hN = 0.0;
  double lM = 1.3;
  double lI = 1.0;

  double singleWeight = 15.0;

  std::vector<double> myWeights(dim, 0.0);
  /*
  if (dim>=12) {
      myWeights[1*dim/5] = singleWeight;
      myWeights[2*dim/5] = singleWeight;
      myWeights[3*dim/5] = singleWeight;
      myWeights[4*dim/5] = singleWeight;
  } else if (dim>=6) {
      myWeights[1*dim/2] = singleWeight;
      myWeights[3*dim/2] = singleWeight;
  } else
  */
  if (dim >= 3) {
    int iW = (dim - 1) / 2;
    cout << "iW =" << iW << endl;
    myWeights[iW] = singleWeight;
  }

  // create the search strategy object
  nloptwr::NLOptWrSStrat sStrategy(sStrat, useGrad);

  cout << "sStrategy.getUseGradient(): " << sStrategy.getUseGradient() << endl;

  // utl::OmpHelper ompHelper;
  ChainWithWeights chainWithWeights(dim, xN, hN, lM, lI, myWeights);

  oif::OptFknBase &oTarget = chainWithWeights;

  nloptwr::NLOptWrapper optWr(oTarget, nThr);
  
  // NOTE: this is the default subalgorithm of AUGLAG
  optWr.setPreferedAlgorithm(nlopt::LD_LBFGS);
  
  // parallel evaluations need less time
  if (useGrad)
    maxTimeSec /= static_cast<int>(optWr.getNThreads());

  optWr.calculateInitialStep(1.3);

  // set population for stochastic optimization
  optWr.setPopulation(1000);

  optWr.setDx(1e-8);
  optWr.setXTolAbs(1e-7);
  optWr.setXTolRel(1e-8);

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

  // save gnuplot file
  ofstream myfile;
  myfile.open("nloptwr_test6.dat");
  chainWithWeights.printResult(optWr.getX(), myfile);
  myfile.close();
  // ========================================================================

  // Display the solution:
  rc = opttest::display(optWr, sStrategy, opt_stat, minf);

  // expected solution:
  bool isOk = true;
  if (dim == 120) {

    double fOptExpcted = -806.770403;
    // test
    // expected solution:
    vector<double> solution{
        -0.210412, -0.207306, -0.204197, -0.201083, -0.197966, -0.194844,
        -0.191719, -0.188591, -0.185458, -0.182321, -0.179182, -0.176038,
        -0.172891, -0.169740, -0.166586, -0.163429, -0.160268, -0.157104,
        -0.153937, -0.150767, -0.147594, -0.144418, -0.141239, -0.138057,
        -0.134872, -0.131684, -0.128494, -0.125301, -0.122106, -0.118908,
        -0.115707, -0.112504, -0.109299, -0.106092, -0.102882, -0.099671,
        -0.096457, -0.093241, -0.090023, -0.086804, -0.083582, -0.080359,
        -0.077134, -0.073908, -0.070680, -0.067451, -0.064219, -0.060987,
        -0.057754, -0.054519, -0.051283, -0.048046, -0.044809, -0.041570,
        -0.038330, -0.035089, -0.031848, -0.028606, -0.025364, -0.022121,
        0.018560,  0.021804,  0.025047,  0.028289,  0.031531,  0.034773,
        0.038013,  0.041253,  0.044492,  0.047730,  0.050967,  0.054203,
        0.057438,  0.060671,  0.063903,  0.067135,  0.070364,  0.073592,
        0.076819,  0.080044,  0.083267,  0.086489,  0.089709,  0.092926,
        0.096143,  0.099356,  0.102568,  0.105778,  0.108986,  0.112191,
        0.115394,  0.118595,  0.121793,  0.124989,  0.128182,  0.131372,
        0.134560,  0.137745,  0.140928,  0.144107,  0.147283,  0.150457,
        0.153628,  0.156795,  0.159959,  0.163120,  0.166278,  0.169432,
        0.172583,  0.175730,  0.178874,  0.182015,  0.185152,  0.188284,
        0.191414,  0.194539,  0.197661,  0.200779,  0.203893,  0.207002};

    isOk = opttest::fvcompare(minf/fOptExpcted, 1.0, optWr.getX(), solution, 0.009);
  } else {
    cout << "The solution (dim!=120) is not checked!" << endl;
  }

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
  int dim = 120;
  int maxTimeSec = 300;
  nloptwr::SSTRAT sStrat = nloptwr::SSTRAT::L;
  bool useGrad = true;

  if (argc <= 2) {
    cout << endl
         << "cout usage:" << endl
         << " " << argv[0] << " [dim] [useGrad] [sStrat] [maxTimeSec] " << endl
         << " # dim=dimension of problem " << endl
         << " # grad=usage of gradients (1 means with gradients (default), "
            "otherwise gradients are not used) "
         << endl
         << " # sStrat: L=Local, LM=Local Meta., ... (default: L) "
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

  if (dim < 1) {
    dim = 1;
  }

  cout << "# dim       :  " << dim << " " << endl;
  cout << "# useGrad   :  " << useGrad << " " << endl;
  cout << "# sStrat    : '" << getStrategyAsString(sStrat) << "' " << endl;
  cout << "# maxTimeSec:  " << maxTimeSec << " " << endl;

  int rc = opttest::opttest6(dim, useGrad, sStrat, maxTimeSec);

  cout << endl;

  return rc;
}
