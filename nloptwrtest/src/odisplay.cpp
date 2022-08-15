
#include "nloptwrtest/odisplay.h"

#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/griewank.h"
#include "nloptwrtest/oproblem.h"

#include "nloptwr/optfktnbase.h"
#include "nloptwr/nloptwrsstrat.h"
#include "nloptwr/nloptwralgorithm.h"

#include <cmath>
#include <vector>

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

namespace opttest {


int displaySearchAlgorithms(const nloptwr::NLOptWrapper& optWr, const nloptwr::NLOptWrSStrat& sStrat ) {

    // print selected algorithms
    std::vector<nloptwr::NLOptWrAlgorithm> algs = optWr.getSelectedAlgorithms ( sStrat );

    int rc=(algs.empty()? 1 : 0);

    cout << endl << "optWr.getSelectedAlgorithms( " << sStrat.toString() << ") = { ";
    for ( size_t i=0; i<algs.size(); i++ ) {
        if ( i>0 ) cout << ", ";
        cout << algs.at ( i ).getName();
    }
    cout << " } " << endl;

    return rc;
}

int displayResult(const nloptwr::NLOptWrapper& optWr, nlopt::result opt_stat, double minf) {
    int rc=0;

    // get result
    vector<double> x = optWr.getX();

    cout << "Solution: " << endl;
    cout << "minF     = " << minf  << endl;
    for ( unsigned int i=0; i< x.size(); i++ ) {
        cout << "x[" << fixed << setw(3) <<i<<"]   = " << setw(6) << x[i] << endl;
    }
    cout << "opt_stat = " << opt_stat << " : "  << optWr.getStringOfResult ( opt_stat ) << endl;
    cout << "minF     = " << minf  << endl;
    cout << " " << endl;
    cout << "rc        = " << rc << endl;
    cout << "optTime   = " << fixed << setw(6) << optWr.getOptTime();

  return rc;
}

int display(const nloptwr::NLOptWrapper& optWr, const nloptwr::NLOptWrSStrat& sStrat, nlopt::result opt_stat, double minf) {
    int rc1=displaySearchAlgorithms(optWr, sStrat);
    int rc2=displayResult(optWr, opt_stat, minf);

    int rc=(rc2!=0)? rc2 : rc1;

    return rc;
}



} // namespace opttest
