
#include "nloptwr/nloptwra.h"

#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/oproblem.h"
#include "nloptwrtest/odisplay.h"
#include "nloptwrtest/vcompare.h"

#include "nloptwr/omphelper.h"

#include <vector>
using namespace std;

namespace opttest {
    
int opttest4()
{
    int rc=0;


    int dim=10;

    // number of threads ("0" means autodetect)
    int nThr=0;

    // timeout of 2nd algorithm
    int maxTimeSec=20;

    int maxEvals=1000000000;

    bool useGrad=true;
    bool useAugLagBeforeMlsl=true;

    // a target function
    Griewank oGriewank( dim ); 
    oGriewank.initialize ( -15.0, 15.0, 14.3 );

    // get reference of the target function
    oif::OptFknBase&  oTarget = oGriewank;

    // get initial x 
    vector<double> x ( oTarget.getXInitial() );

    // create the NLOptWrapper object
    nloptwr::NLOptWrapper optWr ( oTarget, nThr);

    // parallel evaluations need less time
    if (useGrad) maxTimeSec /= static_cast<int>(optWr.getNThreads());
    
    // you can make proposals for the algorithms
    optWr.setPreferedAlgorithm( nlopt::LD_CCSAQ, 0, 700);
    optWr.setPreferedAlgorithm( nlopt::GD_STOGO_RAND, 0, 50);

    // Improvement of solution: 
    cout << "Start of optimization..." << endl;
        
    // select strategy: "L", "LM", "GM", "G" an "R"
    nloptwr::NLOptWrSStrat sStrategy( nloptwr::SSTRAT::GM, useGrad, useAugLagBeforeMlsl );

    nlopt::result opt_stat = nlopt::result::FAILURE; 
    
    try {
        // start opptimization
        opt_stat = optWr.optimize(sStrategy, maxTimeSec, maxEvals);
    } catch (runtime_error eRt) {
        cerr << "ERROR: runtime_error : " << eRt.what() << endl; cerr.flush();
    } catch (exception e) {
        cerr << "ERROR: exception : " << e.what() << endl; cerr.flush();
        throw exception(e);
    }


    // get the optimized value
    double minf = optWr.getLastOptimumValue();

    // =====================================================

    // display the results
    rc = opttest::display(optWr, sStrategy, opt_stat, minf);
    
    // test
    bool isOk = opttest::fvcompare(minf, 0.0, optWr.getX(), 0.0);
    if (!isOk) {
        rc=1;
        cerr << "The optimization has failed!" << endl;
    }

return rc;

}; // main

} // namespace opttest

int main()
{
    int rc=opttest::opttest4 ();

  return rc;
}


