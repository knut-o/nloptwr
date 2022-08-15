
#include "nloptwr/nloptwra.h"

#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/rastrigin.h"
#include "nloptwrtest/oproblem.h"
#include "nloptwrtest/odisplay.h"
#include "nloptwrtest/vcompare.h"

#include <vector>
using namespace std;

namespace opttest {

/**
 * test
 * @param dim dimension of problem
 * @param maxTimeSec maximal time [s]
 * @param sstrat search strategy (default: [G]lobal)
 */
int opttest5(int dim, int maxTimeSec, nloptwr::SSTRAT sstrat)
{
    // return code
    int rc=0;

    // number of threads ("0" means autodetect)
    int nThr=0;

    // maximal fnction evaluations
    int maxEvals=1000000000;

    // use gradient flag
    bool useGrad=true;

    // flag
    bool useAugLagBeforeMlsl=true;

    // select strategy
    nloptwr::NLOptWrSStrat sStrategy( sstrat, useGrad, useAugLagBeforeMlsl );

    // target function
    Rastrigin oRastrigin( dim );
    oRastrigin.initialize ( -5.05, 5.05, 5.0 );

    // get reference of problem
    oif::OptFknBase&  oTarget = oRastrigin;

    // get initial x
    vector<double> x ( oTarget.getXInitial() );

    // create th NLOptWrapper object
    nloptwr::NLOptWrapper optWr ( oTarget, nThr);

    // parallel evaluations need less time
    if (sStrategy.getUseGradient() && useGrad) maxTimeSec /= static_cast<int>(optWr.getNThreads());

    // you can make proposals
    // optWr.setPreferedAlgorithm( nlopt::LD_LBFGS );
    optWr.setPreferedAlgorithm( nlopt::GD_STOGO_RAND, 1, 90);

    optWr.setDx(0.0001);
    optWr.setPopulation(1000);

    // calculte initial steps (in case of methods with no derivation)
    optWr.calculateInitialStep(1.3);

    // start opptimization
    nlopt::result opt_stat = nlopt::result::FAILURE;

    try {
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

    // compare the result
    bool isOk=opttest::vcompare(optWr.getX(), 0.0);
    if (!isOk) {
        rc=1;
        cerr << "The optimization has failed!" << endl;
    }

    return rc;

}; // main

} // namespace opttest

// =====================================================
// =====================================================

int main(int argc, char *argv[])
{
    int dim = 4;
    int maxTimeSec = 300;
    nloptwr::SSTRAT sstrat=nloptwr::SSTRAT::G;

    if (argc<=2) {
        cout
        << endl
        << "cout usage:" << endl
        << " " << argv[0] << " [dim] [maxTimeSec] [G|GM|R] " << endl
        << " # dim=number of parameters (default=" << dim << ")" << endl
        << " # maxTimeSec: max. time [s] (default=" << maxTimeSec << ") " << endl
        << " # G=Global, GM=Global Meta; R=Random (default='" << getStrategyAsString(sstrat) << "')" << endl
        << endl;
    }

    for (int i = 1; i < argc; ++i) {
        if (i == 1) dim=atoi(argv[i]);
        if (i == 2) maxTimeSec = atoi(argv[i]);
        if (i == 3) sstrat = nloptwr::getStrategyFromString(argv[i], nloptwr::SSTRAT::G);
    }

    int rc=opttest::opttest5(dim, maxTimeSec, sstrat);

    cout << endl;

return rc;
}
