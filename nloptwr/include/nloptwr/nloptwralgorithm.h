
#ifndef NLOPT_ALGORITM_H
#define NLOPT_ALGORITM_H

#include "nlopt.hpp"

#include <string>

namespace nloptwr {

/**
 * category of search strategy
 * N = NONE: No supported search strategy
 * L = Local search strategy
 * G = Global search strategy
 * R = Random search strategy
 * GM = Global meta saerch strategy
 * LM = Local meta search strategy
 */
// enum SEARCH_STRATEGY {
enum class SSTRAT : int { GM = -2, LM = -1, N = 0, L = 1, G = 2, R = 3 };

/**
 * get meta search strategy of strategy
 * @param meta search strategy
 * @return search strategy
 */
SSTRAT getMetaSearchStrategy(SSTRAT strat);

/**
 * get non meta search strategy of strategy
 * @param meta search strategy
 * @return non search strategy
 */
SSTRAT getNonMetaSearchStrategy(SSTRAT strat);

/**
 * test if is a local or global or meta search strategy of strategy
 * @param strat search strategy
 * @return result (true means local or global or meta search strategy)
 */
bool isSearchStrategyLGM(SSTRAT strat);

/**
 * test if is a local or global search strategy of strategy
 * @param strat search strategy
 * @return result (true means local or global search strategy)
 */
bool isSearchStrategyLG(SSTRAT strat);

/**
 * test if is a meta search strategy of strategy
 * @param strat search strategy
 * @return result (true means meta search strategy)
 */
bool isMetaSearchStrategy(SSTRAT strat);

/**
 * test if is a stochastic strategy of strategy
 * @param strat search strategy
 * @return result (true means stochastic search strategy)
 */
bool isSearchStrategyR(SSTRAT strat);

/**
 * test if the algorithm is a local or global
 * @param strat search strategy
 * @param searchLocalOrGlobal flag (true if it is local or global search)
 * @return result (true means local or global search strategy)
 */
bool isP4pAlgLocalOrGlobal(SSTRAT strat, bool searchLocalOrGlobal);

/**
 * get string representation of search strategy
 * @return result string representation of search strategy
 */
std::string getStrategyAsString(SSTRAT strat);

/**
 *
 */
SSTRAT getStrategyFromString(const std::string &src,
                             SSTRAT sstratDefault = SSTRAT::G);

/**
 * @class NLOptWrAlgorithm
 * optimization algorithm
 * This class stores properties of the algorithm
 */
class NLOptWrAlgorithm {

public:
  /**
   * constructor
   *  @param a enum of algotitm
   *  @param n name of algorithm
   *  @param rgl ability of handlich stochstic problems
   *  @param hasGrad gradient flag
   *  @param neC ability to handle nonequal constraints
   *  @param necO ability to handle only nonequal constraints
   *  @param nSubopt flag if a subopt is needed
   *  @param minP minimal number of parameters
   *  @param maxP maximal number of parameters
   */
  NLOptWrAlgorithm(nlopt::algorithm a, const std::string &n, SSTRAT rgl,
                   bool hasGrad, bool neC, 
                   // bool necO, 
                   bool nSubopt, unsigned int minP,
                   unsigned int maxP);

  /// default constructor
  NLOptWrAlgorithm() = delete;

  /// copy constructor
  // NLOptWrAlgorithm(const NLOptWrAlgorithm &src);

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
  const std::string &getName() const;

  /**
   * get the ability to handle stochastic problems
   * @return ability to handle stochastic problems (boolean)
   */
  SSTRAT getSearchStrategy() const;

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
  void setMinParameters(unsigned int var = 1);

  /**
   * get maximal number of parameters
   * @return maximal number of parameters
   */
  unsigned int getMaxParameters() const;

  /**
   * set maximal number of parameters
   * @param var maximal number of parameters (0 means unlimited)
   */
  void setMaxParameters(unsigned int var = 0);

  /**
   * set no constraints
   * @param val value (default=true)
   */
  void setHasGradient(bool val = true);

  /**
   * set ne constraints
   * @param val value (default=true)
   */
  void setNeConstraints(bool val = true);

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

  /// comparison operator
  bool operator<(const NLOptWrAlgorithm &rhs) const;

  /// comparison operator
  bool operator==(const NLOptWrAlgorithm &rhs) const;

  /// to string
  std::string toString() const;

private:
  /// enum of algorithm
  nlopt::algorithm alg;

  /// name of algorithm
  std::string name;

  /// ability to handle stochastic problems
  SSTRAT pRGL;

  /// is gradient method
  bool hasGradient;

  /// ability to handle nonequal constraints
  bool neConstraints;
  
  /// only ne constraints
  // bool neOnlyConstraints;
  
  /// subopt required
  bool needsSubOpt;

  /// minimal number of parameters
  unsigned int minParam;

  /// maximal number of parameters
  unsigned int maxParam;
};

} // namespace nloptwr

#endif // NLOPT_ALGORITM_H
