
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "nloptwr/nloptwra.h"

using namespace std;

namespace opttest {

/**
 * Demo01
 * A simple optimization task
 * (implementation of test function)
 */
class Demo01 : public oif::OptFknClass {

public:
  /**
   * constructor
   */
  Demo01() {
    // initialization of limits and bounds
    init(50, 0, 0, -1001.0, 1001.0, -520.0);
  }

  /// destructor
  ~Demo01() {}

  /**
   * implementation of a simple function definition
   * @param x (vector of) function arguments
   * @param c vector of nonequal constraints (c_i <= 0.0)
   * @return value of target function
   */
  void optFktn(const std::vector<double> &x, std::vector<double> &fc) {
    double sum = 0.0;
    double tmp = 0.0;

    // calculation of function value
    for (size_t i = 0; i < getSizeOfX(); i++) {
      tmp = x[i] - (0.5 + static_cast<double>(i));
      sum += tmp * tmp;
    }

    // calculation of constraints
    size_t ine = getSizeOfNeConstraints();
    for (size_t i = 1; i <= ine; i++) {
      fc[i + 1] = 0.0;
    }

    fc[0] = sum;
  }

  /**
   * clone method
   * is important for parallelization
   * @return reference of clone (of this object)
   */
  oif::OptFknBase *clone() const { return (new Demo01(*this)); }
};

// ============================================================================

/**
 * test the optimization
 * @param useGrad true means explicit calculation of gradients
 * @return success
 */
int opttest1(bool useGrad) {
  // return code
  int rc = 0;

  // number of threads ("0" means autodetect)
  int nThr = 0;

  // define time limit
  int maxTimeSec = 120;

  // define limit of calculations
  int maxEvals = 1000000000;

  // result
  double minf = 1.0e33;

  // select strategy: "L", "LM", "GM", "G" and "R"
  nloptwr::NLOptWrSStrat sStrat(nloptwr::SSTRAT::L, useGrad);

  // Instantiate the optimization task
  opttest::Demo01 pDemo01;

  // instantiate the NLOptWrapper
  nloptwr::NLOptWrapper optWr(pDemo01, nThr);

  // parallel evaluations need less time
  if (useGrad)
    maxTimeSec /= static_cast<int>(optWr.getNThreads());

  // start optimization
  nlopt::result opt_stat = optWr.optimize(sStrat, maxTimeSec, maxEvals);

  // get result
  minf = optWr.getLastOptimumValue();

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
       << "optWr.getSelectedAlgorithms( " << sStrat.toString() << ") = { ";
  for (size_t i = 0; i < algs.size(); i++) {
    if (i > 0)
      cout << ", ";
    cout << algs.at(i).getName();
  }
  cout << " } " << endl;

  cout << "optTime   = " << fixed << setw(6) << optWr.getOptTime();
  cout << endl << endl;

  return rc;

}; // main

} // namespace opttest

int main() {
  int rc = 0;

  cout << "Test: " << endl << endl;

  cout << "Optimization with gradients: " << endl;

  rc = opttest::opttest1(true);

  return rc;
}
