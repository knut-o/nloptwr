

#ifndef P4PREF_ALGORITHM2_H
#define P4PREF_ALGORITHM2_H

#include "nloptwralgorithm.h"
#include "nlopt.hpp"

#include <string>

#include "nloptwrsearchalgoparam.h"

namespace nloptwr
{

/**
 * NLOptWrSearchAlgoParam2 (=parameter for prefered algorithm)
 */
class NLOptWrSearchAlgoParam2 : public NLOptWrSearchAlgoParam
{

public:

    /**
     * nondefault constructor
     * @param searchStrat search strategy
     * @param hasConstr has contraints flag
     * @param useGrad use gradient flag
     * @param useAugLagBeforeMlsl use Augmented Lagrangian befoe MLSL flag
     */
    NLOptWrSearchAlgoParam2 (SSTRAT searchStrat, bool hasConstr, bool useGrad=true, bool useAugLagBeforeMlsl=true );

    /**
     * copy constructor
     * @param src source object
     * @param useAugLagBeforeMlsl use Augmented Lagrangian befoe MLSL flag
     */
    NLOptWrSearchAlgoParam2 ( const NLOptWrSearchAlgoParam& src, bool useAugLagBeforeMlsl );

    /**
     * copy constructor
     * @param src source object
     */
    NLOptWrSearchAlgoParam2 ( const NLOptWrSearchAlgoParam2& src );

    /// default constructor
    NLOptWrSearchAlgoParam2() = delete;

    /// destructor
    virtual ~NLOptWrSearchAlgoParam2();

    /**
     * get use augmented lagrangian before MLSL
     * @return value of use augmented lagrangian before MLSL flag
     */
    bool getUseAugLagBeforeMLSL() const;

    /**
     * copmparision operator
     * @param rhs object 
     */
    bool operator < ( const NLOptWrSearchAlgoParam2& rhs ) const;

    /// get content as string
    std::string toString() const;

private:

    /// use gradient flag
    bool useAugLagBeforeMLSL;

};

}

#endif // P4PREF_ALGORITHM2_H

