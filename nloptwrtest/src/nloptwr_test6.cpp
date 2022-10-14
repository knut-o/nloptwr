
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
  XH xhn(dim, 0.0);

  XH xg0(0.0, -3.0);
  XH xgn(dim, -8.0);

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
  stringstream fns;
  fns << "nloptwr_test6_" << ((groundIf) ? (*groundIf).getId() : 0) << ".dat";

  myfile.open(fns.str());
  chainWithWeights.printResult(optWr.getX(), myfile);
  myfile.close();
  // ========================================================================

  // Display the solution:
  rc = opttest::display(optWr, sStrategy, opt_stat, minf);

  // expected solution:
  bool isOk = true;
  if (dim == 120 && sStrat == nloptwr::SSTRAT::L) {

    // expectes results
    vector<double> fOptExpcted = {-806.761, -743.367, -731.047};

    // expected solutions:
    vector<vector<double>> solution{
        {-0.210107, -0.207000, -0.203891, -0.200774, -0.197656, -0.194541,
         -0.191413, -0.188281, -0.185149, -0.182012, -0.178872, -0.175730,
         -0.172579, -0.169426, -0.166275, -0.163120, -0.159957, -0.156791,
         -0.153624, -0.150462, -0.147280, -0.144105, -0.140924, -0.137746,
         -0.134558, -0.131372, -0.128179, -0.124989, -0.121801, -0.118589,
         -0.115393, -0.112190, -0.108984, -0.105770, -0.102570, -0.099354,
         -0.096141, -0.092927, -0.089710, -0.086483, -0.083266, -0.080043,
         -0.076822, -0.073593, -0.070363, -0.067137, -0.063902, -0.060672,
         -0.057434, -0.054205, -0.050970, -0.047732, -0.044494, -0.041244,
         -0.038005, -0.034772, -0.031527, -0.028289, -0.025045, -0.021804,
         -0.018562, 0.022119,  0.025359,  0.028600,  0.031842,  0.035085,
         0.038332,  0.041572,  0.044804,  0.048042,  0.051283,  0.054520,
         0.057757,  0.060982,  0.064216,  0.067445,  0.070680,  0.073908,
         0.077134,  0.080355,  0.083580,  0.086804,  0.090022,  0.093241,
         0.096458,  0.099675,  0.102881,  0.106090,  0.109295,  0.112502,
         0.115705,  0.118908,  0.122107,  0.125303,  0.128490,  0.131677,
         0.134871,  0.138058,  0.141239,  0.144413,  0.147593,  0.150763,
         0.153940,  0.157105,  0.160267,  0.163425,  0.166585,  0.169740,
         0.172888,  0.176037,  0.179181,  0.182319,  0.185457,  0.188589,
         0.191719,  0.194840,  0.197964,  0.201080,  0.204196,  0.207305},
        {-0.240948, -0.234272, -0.227582, -0.220870, -0.214141, -0.207387,
         -0.200620, -0.193836, -0.187027, -0.180204, -0.173367, -0.166514,
         -0.159634, -0.152752, -0.145851, -0.138932, -0.132003, -0.125061,
         -0.118102, -0.111137, -0.104158, -0.097170, -0.090172, -0.083168,
         -0.076159, -0.069132, -0.062107, -0.055071, -0.048027, -0.041643,
         -0.041643, -0.041643, -0.041643, -0.041642, -0.041643, -0.041643,
         -0.041643, -0.041643, -0.041643, -0.041643, -0.041643, -0.041643,
         -0.041643, -0.041643, -0.041643, -0.041643, -0.041643, -0.041643,
         -0.041643, -0.041643, -0.041643, -0.041643, -0.041643, -0.041643,
         -0.041643, -0.041643, -0.041643, -0.041643, -0.041643, -0.041643,
         -0.041643, -0.041643, -0.041643, -0.041643, -0.041643, -0.041643,
         -0.041643, -0.041643, -0.041643, -0.041643, -0.041643, -0.041643,
         -0.041643, -0.040244, -0.033078, -0.025909, -0.018737, -0.011564,
         -0.004389, 0.002790,  0.009963,  0.017136,  0.024310,  0.031477,
         0.038645,  0.045808,  0.052964,  0.060116,  0.067272,  0.074405,
         0.081540,  0.088665,  0.095776,  0.102879,  0.109975,  0.117058,
         0.124130,  0.131196,  0.138236,  0.145272,  0.152285,  0.159293,
         0.166282,  0.173250,  0.180202,  0.187142,  0.194059,  0.200955,
         0.207837,  0.214696,  0.221534,  0.228356,  0.235155,  0.241930,
         0.248675,  0.255406,  0.262117,  0.268799,  0.275455,  0.282084},
        {-0.222279, -0.215583, -0.208859, -0.202130, -0.195370, -0.188599,
         -0.181805, -0.175002, -0.168181, -0.161343, -0.154483, -0.147613,
         -0.140730, -0.133835, -0.126917, -0.119998, -0.113066, -0.106124,
         -0.099168, -0.092204, -0.085227, -0.078249, -0.071261, -0.064261,
         -0.057260, -0.050253, -0.043241, -0.042766, -0.044044, -0.045292,
         -0.046508, -0.047693, -0.048845, -0.049963, -0.051047, -0.052096,
         -0.053110, -0.054087, -0.055027, -0.055929, -0.056793, -0.057618,
         -0.058404, -0.059150, -0.059855, -0.060520, -0.061143, -0.061725,
         -0.062264, -0.062761, -0.063215, -0.063627, -0.063994, -0.064319,
         -0.064599, -0.064836, -0.065028, -0.065176, -0.065280, -0.065339,
         -0.065354, -0.065325, -0.065250, -0.065132, -0.064969, -0.064762,
         -0.064511, -0.064216, -0.063877, -0.063495, -0.057053, -0.049873,
         -0.042687, -0.035493, -0.028297, -0.021097, -0.013901, -0.006697,
         0.000507,  0.007709,  0.014911,  0.022113,  0.029312,  0.036506,
         0.043694,  0.050886,  0.058068,  0.065240,  0.072410,  0.079573,
         0.086728,  0.093870,  0.101004,  0.108129,  0.115248,  0.122347,
         0.129437,  0.136510,  0.143573,  0.150625,  0.157657,  0.164675,
         0.171677,  0.178660,  0.185622,  0.192574,  0.199506,  0.206417,
         0.213303,  0.220177,  0.227022,  0.233851,  0.240656,  0.247441,
         0.254198,  0.260931,  0.267647,  0.274328,  0.280995,  0.287628}};

    isOk = opttest::fvcompare(minf / fOptExpcted[nrOfGround], 1.0, optWr.getX(),
                              solution[nrOfGround], 0.009);
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
  int dim = 120;
  int maxTimeSec = 300;
  nloptwr::SSTRAT sStrat = nloptwr::SSTRAT::L;
  bool useGrad = true;
  int noGround = 0;

  if (argc <= 2) {
    cout << endl
         << "cout usage:" << endl
         << " " << argv[0]
         << " [dim] [noGround] [useGrad] [sStrat] [maxTimeSec] " << endl
         << " # dim=dimension of problem " << endl
         << " # noGround=number of ground (0=no ground, 1=linear ground, 2=cos "
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
