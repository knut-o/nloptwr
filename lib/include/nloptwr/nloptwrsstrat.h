

#ifndef NLOPTWR_SSTRAT_H
#define NLOPTWR_SSTRAT_H

#include "nlopt.hpp"
#include "nloptwralgorithm.h"

#include <string>

namespace nloptwr {

/**
 * NloptWrSStrat (=parameter for preferred algorithm)
 */
class NLOptWrSStrat {

public:
  /**
   * nondefault constructor
   * @param searchStrat search strategy
   * @param useGrad use gradient flag
   */
  NLOptWrSStrat(SSTRAT searchStrat, bool useGrad = true);

  /// default constructor
  NLOptWrSStrat() = delete;

  /// copy constructor
  NLOptWrSStrat(const NLOptWrSStrat &src);

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
   * get string representation
   * @return string representation
   */
  std::string toString() const;

  /**
   * set search strategy
   * @param val search strategy
   */
  void setSearchStrategy(SSTRAT val);

protected:
  /// search strategy
  SSTRAT searchStrategy;

  /// use gradient flag
  bool useGradient;
};

} // namespace nloptwr

#endif // NLOPTWR_SSTRAT_H
