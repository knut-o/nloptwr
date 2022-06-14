
#ifndef NLOPT_ALGORITM_H
#define NLOPT_ALGORITM_H

#include "nlopt.hpp"

namespace nloptwr
{

/**
 * category of search strategy
 * N = NONE: No supported search strategy
 * L = Local search strategy
 * G = Gloabal search strategy
 * R = Random serach strategy
 * GM = Global meta saerch strategy
 * LM = Local meta search strategy
 */
enum SEARCH_STRATEGY {
    GM = -2,
    LM = -1,
    N = 0,
    L = 1,
    G = 2,
    R = 3
};

/**
 * get meta search strategy of strategy
 * @param meta search strategy
 * @return search strategy
 */
SEARCH_STRATEGY getMetaSearchStrategy ( SEARCH_STRATEGY strat ) ;

/**
 * get non meta search strategy of strategy
 * @param meta search strategy
 * @return non search strategy
 */
SEARCH_STRATEGY getNonMetaSearchStrategy ( SEARCH_STRATEGY strat );

/**
 * test if is a local or global or meta search strategy of strategy
 * @param strat search strategy
 * @return result (true means local or global or meta search strategy)
 */
bool isSearchStrategyLGM ( SEARCH_STRATEGY strat );

/**
 * test if is a local or global search strategy of strategy
 * @param strat search strategy
 * @return result (true means local or global search strategy)
 */
bool isSearchStrategyLG ( SEARCH_STRATEGY strat );

/**
 * test if is a meta search strategy of strategy
 * @param strat search strategy
 * @return result (true means meta search strategy)
 */
bool isMetaSearchStrategy ( SEARCH_STRATEGY strat );

/**
 * test if is a stochastic strategy of strategy
 * @param strat search strategy
 * @return result (true means stocastic search strategy)
 */
bool isSearchStrategyR ( SEARCH_STRATEGY strat );

/**
 * test if the algoritm is a local or global
 * @param strat search strategy
 * @param searchLocalOrGlobal flag (true if it is local or global search)
 * @return result (true means local or global search strategy)
 */
bool  isP4pAlgLocalOrGlobal ( SEARCH_STRATEGY strat, bool searchLocalOrGlobal );

/**
 * get string representation of search strategy
 * @return result string representation of search strategy
 */
std::string getStrategyAsString ( SEARCH_STRATEGY strat );

/**
 * @class NLOptWrAlgorithm
 * optimiztion algoritm
 * This class stores properties of the algorithm
 */
class NLOptWrAlgorithm
{

public:


   /**
    * constructor
    *  @param a enum of algotitm
    *  @param n name of algorithm
    *  @param rgl ability of handlich stochstic problems
    *  @param hasGrad gradient flag
    *  @param neC ability to handle nonequal constraints
    *  @param nSubopt flag if a subopt is needed
    *  @param minP minimal number of parameters
    *  @param maxP maximal number of parameters
    */
    NLOptWrAlgorithm (
        nlopt::algorithm a,
        const std::string& n,
        SEARCH_STRATEGY rgl,
        bool hasGrad,
        bool neC,
        bool nSubopt,
        unsigned int minP,
        unsigned int maxP
    );

    /// default constructor
    NLOptWrAlgorithm();

    /// copy constructor
    NLOptWrAlgorithm ( const NLOptWrAlgorithm& src );

    /// destructor
    ~NLOptWrAlgorithm();



    /**
     * get enum of algorithm
     * @return enum of algorithm
     */
    nlopt::algorithm getAlgorithmEnum() const;


    /**
     * get name of algorithm
     * @return name of algorithm
     */
    const std::string& getName() const;


    /**
     * get the ability to handle stochastic problems
     * @return ability to handle stochastic problems (boolean)
     */
    SEARCH_STRATEGY getSearchStrategy() const;

    /**
     * get use gradient flag
     * @return use gradient flag
     */
    bool getUseGradient() const;



    /**
     * get the ability to handle nonequal constraints
     * @return ability to handle nonequal constraints (boolean)
     */
    bool canHandleNonEqualConstraints() const;


    /**
     * get the subopt required flag
     * @return the subopt required flag (boolean)
     */
    bool getNeedsSubopt() const;


    /**
     * get minimal number of parameters
     * @return minimal number of parameters
     */
    unsigned int getMinParameters() const;


    /**
     * set minimal number of parameters
     * @param var minimal number of parameters
     */
    void setMinParameters ( unsigned int var=1 );


    /**
     * get maximal number of parameters
     * @return maximal number of parameters
     */
    unsigned int getMaxParameters() const;

    /**
     * set maximal number of parameters
     * @param var maximal number of parameters (0 means unlimited)
     */
    void setMaxParameters ( unsigned int var=0 );


    /**
     * set no constraints
     * @param val value (default=true)
     */
    void setHasGradient ( bool val=true );

    /**
     * set ne constraints
     * @param val value (default=true)
     */
    void setNeConstraints ( bool val=true );

    /**
     * check Augmented Lagrangian
     */
    bool isAugLag() const;

    /**
     * check MSL
     */
    bool isMLSL() const;

    /**
     * test of set population method
     * @return true if population is set, otherwise false
     */
    bool isSetPopulation() const;
    
    /**
     * test if has inner max evaluations
     * @return true if it is set, otherwise false
     */
    bool isHasInnerMaxeval() const;
    
    /**
     * test vector storage call
     * @return true if vector storage call is already invoked
     */
    bool isVectorStorage() const;

    /// comparision operator
    bool operator < ( const NLOptWrAlgorithm& rhs ) const;

    /// comparision operator
    bool operator == ( const NLOptWrAlgorithm& rhs ) const;

    /// to string
    std::string toString() const;

private:
    /// enum of algorithm
    nlopt::algorithm alg;

    /// name of algorithm
    std::string name;

    /// ability to handle stochastic problems
    SEARCH_STRATEGY pRGL;

    /// is gradient method
    bool hasGradient;

    /// ability to handle nonequal constraints
    bool neConstraints;

    /// subopt required
    bool needsSubOpt;

    /// minimal number of parameters
    unsigned int minParam;

    /// maximal number of parameters
    unsigned int maxParam;
};

}

#endif // NLOPT_ALGORITM_H
