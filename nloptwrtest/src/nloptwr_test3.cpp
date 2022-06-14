
#include "nloptwr/nloptwra.h"

#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/oproblem.h"
#include "nloptwrtest/odisplay.h"
#include "nloptwrtest/vcompare.h"

#include <vector>
using namespace std;

namespace opttest {
    
int opttest3()
{
    int rc=0;

    // the result variable
    nlopt::result opt_stat=nlopt::result::FAILURE;

    int dim=3000;

    // number of threads ("0" means autodetect)
    int nThr=0;

    // timeout of algorithm
    int maxTimeSec=120;

    // maximal count of evaluations
    int maxEvals=1000000000;

    // use gradient
    bool useGrad=true;
    
    // other parameter
    bool useAugLagBeforeMlsl=true;

    // the minimum objective value, upon return
    double minf=1.0e33; 

    // =====================================================

    // a target function
    Griewank oGriewank( dim ); 
    
    // initial point is near the optimum
    oGriewank.initialize ( -15.0, 15.0, 2.0 );

    // it is used a local search (because of the speed)
    nloptwr::NLOptWrSStrat sStrat2( nloptwr::L, useGrad, useAugLagBeforeMlsl );

    // =====================================================

    // get reference of the target function
    oif::OptFknBase&  oTarget = oGriewank;

    // create the NLOptWrapper object
    nloptwr::NLOptWrapper optWr ( oTarget, nThr);

    // parallel evaluations need less time
    if (useGrad) maxTimeSec /= static_cast<int>(optWr.getNThreads());
    
    // optimize it
    opt_stat = optWr.optimize(minf, sStrat2, maxTimeSec, maxEvals);

    // =====================================================
    
    // display the result
    rc = opttest::display(optWr, sStrat2, opt_stat, minf);
    
    // check the results
    bool isOk=opttest::vcompare(optWr.getX(), 0.0, 0.0005);
    if (!isOk) {
        rc=1;
        cerr << "The 2nd optimization has failed!" << endl;
    }

return rc;

}; // main

} // namespace opttest

// =====================================================

int main()
{
    int rc=opttest::opttest3 ();

  return rc;
}


