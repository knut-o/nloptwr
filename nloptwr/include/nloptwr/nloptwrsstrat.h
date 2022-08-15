

#ifndef NLOPTWR_SSTRAT_H
#define NLOPTWR_SSTRAT_H

#include "nloptwralgorithm.h"
#include "nlopt.hpp"

#include <string>

namespace nloptwr
{

/**
 * NloptWrSStrat (=parameter for prefered algorithm)
 */
class NLOptWrSStrat
{

public:

    /**
     * nondefault constructor
     * @param searchStrat search strategy
     * @param useGrad use gradient flag
     * @param useAugLagBeforeMlsl use Augmented Lagrangian befor MLSL flag
     */
    NLOptWrSStrat (SSTRAT searchStrat, bool useGrad=true, bool useAugLagBeforeMlsl=true );

    /// default constructor
    NLOptWrSStrat() = delete;

    /// copy constructor
    NLOptWrSStrat ( const NLOptWrSStrat& src );

    /// destructor
    virtual ~NLOptWrSStrat();

    /**
     * get search strategy
     * @ return search strategy
     */
    SSTRAT getSearchStrategy() const;

    /**
     * get use gradient flag
     * @return true if use gradient, otherwise false
     */
    bool getUseGradient() const;

    /**
     * get needs sub optimization flag
     * @return true if subopt is used, otherwise false
     */
    bool getNeedsSubOpt() const;

    /**
     * get use Augmented Lagrangian before MLSL flag
     * @return true if Augmented Lagrangian before MLSL flag is active, otherwise false
     */
    bool getUseAugLagBeforeMLSL() const;

    /**
     * get string representation
     * @return string representation
     */
    std::string toString() const;

    /**
     * set search strategy
     * @param val search strategy
     */
    void setSearchStrategy (SSTRAT val );

protected:

    /// search strategy
    SSTRAT searchStrategy;

    /// use gradient flag
    bool useGradient;

    /// use Augmented Lagrangian before MLSL
    bool useAugLagBeforeMLSL;  // TODO: Parameter in signature

};

}

#endif // NLOPTWR_SSTRAT_H
