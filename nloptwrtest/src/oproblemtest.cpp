

#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/griewank.h"
#include "nloptwrtest/oproblem.h"

#include "nloptwr/nloptwr.h"
#include "nloptwr/nloptwralgorithm.h"
#include "nloptwr/nloptwrsstrat.h"
#include "nloptwr/optfktnbase.h"

#include <cmath>
#include <vector>

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

// extern int auglag_verbose;
// extern int ccsa_verbose;
// extern int mma_verbose;
// extern int testfuncs_verbose;

namespace opttest {

int oproblemtest(bool log, int dim) {

  int rc = 0;

  // number of threads ("0" means autodetect)
  int nThr = 0;

  // bool isStochastic=false; bool isAugLag = true; bool useGrad = true;
  int maxTimeSec = 40;
  int maxEvals = 1000000000;

  // utl::OmpHelper ompHelper;
  // OProblem oProblem ( dim ); oProblem.initialize ( 0.15, 5.0, 0.2 );
  Griewank oProblem(dim);
  oProblem.initialize(-15.0, 15.0, 14.3);

  cout << "TEST" << endl;

  unsigned long ulSize =
      static_cast<unsigned long>(oProblem.getUpperBounds().size());
  cout << "### oProblem.getUpperBounds()=" << ulSize << endl;

  cout << "TEST0" << endl;

  // vector<double> ub(oProblem.getUpperBounds());
  // vector<double> lb(oProblem.getLowerBounds());
  vector<double> x(oProblem.getXInitial());

  // vector<double> initial_step(dim, 0.1);
  // vector<double> sub_initial_step (dim, 0.1);
  // vector<double> tol(dim, 1.0e-8);

  nloptwr::NLOptWrapper optWr(oProblem, nThr);

  optWr.setXTolRel(0.00000003);
  optWr.setXTolAbs(0.000001);
  // optWr.setXTolRel(0.003);
  // optWr.setTolerance(0.001);

  // NOTE: Non-Gradient methods do not need a numerical derivation!!!!!!
  bool useGrad = true;

  nloptwr::NLOptWrSStrat sStratL(nloptwr::SSTRAT::L, useGrad);
  nloptwr::NLOptWrSStrat sStratLM(nloptwr::SSTRAT::LM, useGrad);
  nloptwr::NLOptWrSStrat sStratR(nloptwr::SSTRAT::R, useGrad);
  nloptwr::NLOptWrSStrat sStratGM(nloptwr::SSTRAT::GM, useGrad);
  nloptwr::NLOptWrSStrat sStratG(nloptwr::SSTRAT::G, useGrad);

  nloptwr::NLOptWrSStrat &sStrat = sStratL;

  std::vector<nloptwr::NLOptWrAlgorithm> algs0 =
      optWr.getSelectedAlgorithms(sStrat);

  cout << endl
       << "optWr.getSelectedAlgorithms( " << sStrat.toString() << ") = { ";
  for (size_t i = 0; i < algs0.size(); i++) {
    if (i > 0)
      cout << ", ";
    cout << algs0.at(i).getName();
  }
  cout << " } " << endl;

  if (true) {
    // if (sStrat.getSearchStrategy()==nloptwr::R)
    optWr.calculateInitialStep(1.3);

    nlopt::result opt_stat =
        optWr.optimize(sStrat, // isStochastic, isAugLag, useGrad,
                       maxTimeSec, maxEvals);

    double minf = optWr.getLastOptimumValue();

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

    x = optWr.getX();

    rc = (opt_stat >= 0) ? 0 : opt_stat;

    cout << "Solution: " << endl;
    if (log) {
      for (unsigned int i = 0; i < x.size(); i++) {
        cout << "x[" << fixed << setw(3) << i << "]   = " << setw(6) << x[i]
             << endl;
      }
      cout << "opt_stat = " << opt_stat << " : "
           << optWr.getStringOfResult(opt_stat) << endl;
      cout << "minf     = " << minf << endl;
      cout << " " << endl;
      cout << "rc        = " << rc << endl;
      cout << "optTime   = " << fixed << setw(6) << optWr.getOptTime() << endl;
    }
  }

  return rc;
};

} // namespace opttest
