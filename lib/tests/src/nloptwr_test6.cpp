
#include "nloptwr/nloptwra.h"

#include "nloptwrtest/chainwithweights.h"

#include "nloptwrtest/griewank.h"
#include "nloptwrtest/odisplay.h"
#include "nloptwrtest/vcompare.h"

#include "nloptwrtest/groundcos.h"
#include "nloptwrtest/groundif.h"
#include "nloptwrtest/groundlin.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

namespace opttest {

int opttest6(int dim, int nrOfGround = 0, bool useGrad = 1,
             nloptwr::SSTRAT sStrat = nloptwr::SSTRAT::L,
             int maxTimeSec = 400) {
  // return code
  int rc = 0;

  // number of threads ("0" means autodetect)
  int nThr = 0;

  int maxEvals = 1000000000;

  XH xh0(0.0, 0.0);
  XH xhn(dim + 1, 0.0);

  XH xg0(0.0, (-12.0 / 121.0) * (dim + 1));
  XH xgn(dim + 1, (-50.0 / 121.0) * (dim + 1));

  double lM = 1.3;
  double lI = 1.0;

  double singleWeight = 15.0;

  std::vector<double> myWeights(dim + 1, 0.0);
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
    int iW = (dim + 1) / 2;
    cout << "iW =" << iW << endl;
    myWeights[iW] = singleWeight;
  }

  // create the search strategy object
  nloptwr::NLOptWrSStrat sStrategy(sStrat, useGrad);

  cout << "sStrategy.getUseGradient(): " << sStrategy.getUseGradient() << endl;

  shared_ptr<GroundIf> groundIf;

  if (nrOfGround == 1) {
    GroundLin groundLin(xg0, xgn);
    groundIf = shared_ptr<GroundIf>(groundLin.clone());
  } else if (nrOfGround == 2) {
    GroundCos groundCos(xg0, xgn);
    groundIf = shared_ptr<GroundIf>(groundCos.clone());
  } else if (nrOfGround != 0) {
    cerr << "Value nrOfGround is out of range" << endl;
  }

  // utl::OmpHelper ompHelper;
  ChainWithWeights chainWithWeights(dim, xhn, lM, lI, myWeights,
                                    groundIf.get());

  oif::OptFknBase &oTarget = chainWithWeights;

  nloptwr::NLOptWrapper optWr(oTarget, nThr);

  // NOTE: Only few algorithms are working with AUGLAG here
  optWr.setPreferedAlgorithm(nlopt::LD_SLSQP); // OK

  // parallel evaluations need less time
  if (useGrad)
    maxTimeSec /= static_cast<int>(optWr.getNThreads());

  optWr.calculateInitialStep(1.3);

  // set population for stochastic optimization
  optWr.setPopulation(10 * dim);

  // optWr.setDx(1e-8);

  // NOTE: This tolerance is important:
  optWr.setXTolAbs(1e-6);

  // optWr.setXTolRel(1e-6);
  optWr.setTolMConstraints(8.0E-5);

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
  stringstream fns;
  fns << "nloptwr_test6_" << ((groundIf) ? (*groundIf).getId() : 0) << ".dat";

  myfile.open(fns.str());
  chainWithWeights.printResult(optWr.getX(), myfile);
  fns << "# opt_stat = " << opt_stat << endl;
  fns << "# minf = " << setw(8) << minf << endl;
  myfile.close();
  // ========================================================================

  // Display the solution:
  rc = opttest::display(optWr, sStrategy, opt_stat, minf);

  // expected solution:
  bool isOk = true;
  if (dim == 119) {

    // expects results
    vector<double> fOptExpcted = {-4938.24, -4261.37, -4247.22};

    vector<double> v;
    isOk = opttest::fvcompare(minf / fOptExpcted[nrOfGround], 1.0, v, v, 0.09);
  } else {
    cout << "The solution (dim!=120 || sStrat != nloptwr::SSTRAT::L) is not "
            "checked!"
         << endl;
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
  int dim = 119;
  int maxTimeSec = 300;
  nloptwr::SSTRAT sStrat = nloptwr::SSTRAT::L;
  bool useGrad = true;
  int noGround = 0;

  if (argc <= 2) {
    cout << endl
         << "cout usage:" << endl
         << " " << argv[0]
         << " [dim] [noGround] [useGrad] [sStrat] [maxTimeSec] " << endl
         << " # dim=dimension of problem (default=" << dim << ")" << endl
         << " # noGround=number of ground (0=no ground (default), 1=linear "
            "ground, 2=cos "
            "ground), "
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
    if (i == 2) {
      if (string(argv[i]) == "1")
        noGround = 1;
      if (string(argv[i]) == "2")
        noGround = 2;
    }
    if (i == 3)
      useGrad = (string(argv[i]) == "1");
    if (i == 4)
      sStrat = nloptwr::getStrategyFromString(argv[i]);
    if (i == 5)
      maxTimeSec = atoi(argv[i]);
  }

  if (dim < 1) {
    dim = 1;
  }

  cout << "# dim       :  " << dim << " " << endl;
  cout << "# noGround  :  " << noGround << " " << endl;
  cout << "# useGrad   :  " << useGrad << " " << endl;
  cout << "# sStrat    : '" << getStrategyAsString(sStrat) << "' " << endl;
  cout << "# maxTimeSec:  " << maxTimeSec << " " << endl;

  int rc = opttest::opttest6(dim, noGround, useGrad, sStrat, maxTimeSec);

  cout << endl;

  return rc;
}
