
#include "nloptwr/nloptwra.h"

#include "nloptwrtest/odisplay.h"
#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/griewank.h"
#include "nloptwrtest/oproblem.h"
#include "nloptwrtest/vcompare.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

// extern int auglag_verbose;
// extern int ccsa_verbose;
// extern int mma_verbose;
// extern int testfuncs_verbose;

namespace opttest {

int opttest2()
{
    // return code
    int rc=0;

    // dimension of proble
    int dim=50;


    // number of threads ("0" means autodetect)
    int nThr=0;

    // bool isStochastic=false; bool isAugLag = true; bool useGrad = true;
    int maxTimeSec=60;

    int maxEvals=1000000000;

    // use gradient methods
    bool useGrad=true;
    
    // prefer Augmented Lagrangian befoe MLSL
    bool useAugLagBeforeMlsl=true;

    // the initial minimum objective
    double minf=1.0e33; 

    // select strategy: "L", "LM", "GM", "G" an "R"
    nloptwr::NLOptWrSStrat sStrat( nloptwr::LM, useGrad, useAugLagBeforeMlsl );

    // utl::OmpHelper ompHelper;
    OProblem oProblem ( dim ); 

    oif::OptFknBase&  oTarget = oProblem;

    nloptwr::NLOptWrapper optWr ( oTarget, nThr);

    // parallel evaluations need less time
    if (useGrad) maxTimeSec /= static_cast<int>(optWr.getNThreads());
    
    // avoid smooth with threads in case of random methods
    if (sStrat.getSearchStrategy()==nloptwr::R) optWr.setNewDerivMethod(false);

    // optWr.calculateInitialStep(1.3);

    // start opptimization
    nlopt::result opt_stat = optWr.optimize(minf, sStrat, maxTimeSec, maxEvals);
    
    // ========================================================================
    
    // Display the solution:
    rc = opttest::display(optWr, sStrat, opt_stat, minf);

    // test the solution:
    vector<double> solution(dim, 1.0);
    solution[0]=1.0/3.0;
    solution[1]=8.0/27.0;
    bool isOk=opttest::vcompare(optWr.getX(), solution, 0.005);
    if (!isOk) {
        rc=1;
        cerr << "The optimization has failed!" << endl;
    }

    return rc;

}; // main

} // namespace opttest


int main()
{
    int rc=0;

    rc=opttest::opttest2 ();

    return rc;
}


