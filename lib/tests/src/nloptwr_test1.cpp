
// include all header files at once
#include "nloptwr/nloptwra.h"
#include "nloptwrtest/vcompare.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

namespace opttest {

/**
 * PDemo01
 * A simple optimization task
 * (implementation of test function)
 */
class PDemo01 : public oif::OptFknClass {

public:
  /**
   * constructor
   */
  PDemo01() {
    // initialization of limits and bounds
    init(5, 0, 0, -1001.0, 1001.0, -520.0);
  }

  /// destructor
  ~PDemo01() {}

  /**
   * implementation of a simple function definition
   * @param x vector of arguments
   * @param cf vector of target function f, equality constraints (c_i = 0.0),
   * and nonequality constraints (c_i <= 0.0)
   */
  void optFktn(const std::vector<double> &x, std::vector<double> &fc) {
    double sum = 0.0;
    double tmp = 0.0;

    // calculation of function value
    for (size_t i = 0; i < getSizeOfX(); i++) {
      tmp = x[i] - (0.5 + static_cast<double>(i));
      sum += tmp * tmp;
    }

    fc[0] = sum;
  }

  /**
   * clone method
   * is important for parallelization
   * @return reference of clone (of this object)
   */
  oif::OptFknBase *clone() const { return (new PDemo01(*this)); }
};

// ============================================================================

/**
 * test the optimization
 * @param useGrad true means explicit calculation of gradients
 * @param sStrat search strategy
 * @param maxTimeSec max time linit
 * @return success
 */
// int opttest1(bool useGrad)
int opttest1(bool useGrad, nloptwr::SSTRAT sStrat = nloptwr::SSTRAT::L,
             int maxTimeSec = 400) {
  // return code
  int rc = 0;

  // number of threads ("0" means autodetect)
  int nThr = 0;

  // define limit of calculations
  int maxEvals = 1000000000;

  // Instantiate the optimization task
  opttest::PDemo01 pDemo01;

  // create the search strategy object
  nloptwr::NLOptWrSStrat sStrategy(sStrat, useGrad);

  // instantiate the NLOptWrapper
  nloptwr::NLOptWrapper optWr(pDemo01, nThr);

  // parallel evaluations need less time
  if (useGrad)
    maxTimeSec /= static_cast<int>(optWr.getNThreads());

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

  // =====================================================

  // get the solution
  vector<double> x = optWr.getX();

  // display the result
  cout << "Solution: " << endl;
  for (unsigned int i = 0; i < x.size(); i++) {
    cout << "x[" << fixed << setw(3) << i << "]   = " << setw(6) << x[i]
         << endl;
  }
  cout << "opt_stat = " << opt_stat << " : "
       << optWr.getStringOfResult(opt_stat) << endl;
  cout << "minF     = " << minf << endl;
  cout << "rc        = " << rc << endl;

  // print the selected algorithms
  std::vector<nloptwr::NLOptWrAlgorithm> algs =
      optWr.getSelectedAlgorithms(sStrat);
  cout << endl
       << "optWr.getSelectedAlgorithms( " << getStrategyAsString(sStrat)
       << ") = { ";
  for (size_t i = 0; i < algs.size(); i++) {
    if (i > 0)
      cout << ", ";
    cout << algs.at(i).getName();
  }
  cout << " } " << endl;

  cout << "optTime   = " << fixed << setw(6) << optWr.getOptTime();
  cout << endl << endl;

  vector<double> solution(x.size());
  for (size_t i = 0; i < x.size(); i++)
    solution[i] = 0.5 + static_cast<double>(i);
  bool isOk = opttest::fvcompare(minf, 0.0, optWr.getX(), solution);
  if (!isOk) {
    rc = 1;
    cout.flush();
    cerr << "The optimization has failed!" << endl;
    cerr.flush();
  }

  return rc;

}; // main

} // namespace opttest

// =====================================================
// =====================================================

int main(int argc, char *argv[]) {
  int rc = 0;
  int maxTimeSec = 300;
  nloptwr::SSTRAT sStrat = nloptwr::SSTRAT::L;
  bool useGrad = true;

  if (argc <= 2) {
    cout << endl
         << "cout usage:" << endl
         << " " << argv[0] << " [useGrad] [sStrat] [maxTimeSec] " << endl
         << " # grad=usage of gradients (1 means with gradients (default), "
            "otherwise gradients are not used) "
         << endl
         << " # sStrat: L=Local, LM=Local Meta, G=Global, GM=Global Meta; "
            "R=Random (default='"
         << getStrategyAsString(sStrat) << "')" << endl
         << " # maxTimeSec: max. time [s] (default=" << maxTimeSec << ") "
         << endl
         << endl;
  }

  for (int i = 1; i < argc; ++i) {
    if (i == 1)
      useGrad = (string(argv[i]) == "1");
    if (i == 2)
      sStrat = nloptwr::getStrategyFromString(argv[i], nloptwr::SSTRAT::L);
    if (i == 3)
      maxTimeSec = atoi(argv[i]);
  }

  rc = opttest::opttest1(useGrad, sStrat, maxTimeSec);

  cout << endl;

  return rc;
}
