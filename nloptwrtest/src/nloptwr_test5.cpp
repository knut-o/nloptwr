
#include "nloptwr/nloptwra.h"

#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/rastrigin.h"
#include "nloptwrtest/oproblem.h"
#include "nloptwrtest/odisplay.h"
#include "nloptwrtest/vcompare.h"

#include <vector>
using namespace std;

namespace opttest {
    
int opttest5()
{
    // return code
    int rc=0;

    // number of parameters
    int dim=80;

    // number of threads ("0" means autodetect)
    int nThr=0;

    // maximal execution time
    int maxTimeSec=80;
 
    // maximal fnction evaluations
    int maxEvals=1000000000;

    // use gradient flag
    bool useGrad=true;
    
    // flag
    bool useAugLagBeforeMlsl=true;

    // the minimum objective value, upon return
    double minf=1.0e33; 

    // select strategy: "L", "LM", "GM", "G" an "R"
    nloptwr::NLOptWrSStrat sStrat( nloptwr::GM, useGrad, useAugLagBeforeMlsl );

    // target function
    Rastrigin oRastrigin( dim ); 
    oRastrigin.initialize ( -5.1, 5.1, 5.0 );

    // get reference of problem
    oif::OptFknBase&  oTarget = oRastrigin;

    // get initial x 
    vector<double> x ( oTarget.getXInitial() );

    // create th NLOptWrapper object
    nloptwr::NLOptWrapper optWr ( oTarget, nThr);

    // parallel evaluations need less time
    if (useGrad) maxTimeSec /= static_cast<int>(optWr.getNThreads());
    
    // you can make proposals
    optWr.setPreferedAlgorithm( nlopt::LD_LBFGS );
    optWr.setPreferedAlgorithm( nlopt::GD_STOGO_RAND, 0, 90);

    // calculte initial steps (in case of methods with no derivation)
    optWr.calculateInitialStep(1.3);

    // start optimization
    nlopt::result opt_stat = optWr.optimize(minf, sStrat, maxTimeSec, maxEvals);
    
    // display the results
    rc = opttest::display(optWr, sStrat, opt_stat, minf);
    
    // compare the result
    bool isOk=opttest::vcompare(optWr.getX(), 0.0);
    if (!isOk) {
        rc=1;
        cerr << "The optimization has failed!" << endl;
    }

    return rc;

}; // main

} // namespace opttest

int main()
{
    int rc=opttest::opttest5();

  return rc;
}






