

#ifndef P4PREF_ALGORITHM_H
#define P4PREF_ALGORITHM_H

#include "nlopt.hpp"
#include "nloptwralgorithm.h"

#include <string>

namespace nloptwr {

/**
 * NLOptWrSearchAlgoParam (=parameter for preferred algorithm)
 */
class NLOptWrSearchAlgoParam {

public:
  /**
   * nondefault constructor
   * @param searchStrat search strategy
   * @param hasConstr has constraints flag
   * @param useGrad use gradient flag
   */
  NLOptWrSearchAlgoParam(SSTRAT searchStrat, bool hasEqConstr, bool hasNeConstr,
                         bool useGrad = true);

  /**
   * special copy constructor to change the strategy
   * @param searchStrat search strategy
   * @param src object to be cloned except search strategy
   */
  NLOptWrSearchAlgoParam(SSTRAT searchStrat, const NLOptWrSearchAlgoParam &src);

  /// default constructor
  NLOptWrSearchAlgoParam();

  /// copy constructor
  NLOptWrSearchAlgoParam(const NLOptWrSearchAlgoParam &src);

  /// destructor
  virtual ~NLOptWrSearchAlgoParam();

  /// coparision operator
  bool operator<(const NLOptWrSearchAlgoParam &rhs) const;

  /// get search strategy
  SSTRAT getSearchStrategy() const;

  /// get has constraints flag
  bool getHasContraints() const;

  /// get has constraints flag
  bool getHasEqContraints() const;

  /// get has constraints flag
  bool getHasNeContraints() const;

  /// get use gradient flag
  bool getUseGradient() const;

  /// get needs sub optimization flag
  bool getNeedsSubOpt() const;

  /// set use eqzality constraints
  void setUseEqContraints(bool val = true);

  /// set use noequality constraints
  void setUseNeContraints(bool val = true);

  /// string representation
  std::string toString() const;

  /// string representation (short)
  std::string toStringShort() const;

  /// set search strategy
  void setSearchStrategy(SSTRAT val);

protected:
  /// search strategy
  SSTRAT searchStrategy;

  /// has equality constrauints flag
  bool hasEqContraints;

  /// has equality constrauints flag
  bool hasNeContraints;

  /// use gradient flag
  bool useGradient;
};

} // namespace nloptwr

#endif // P4PREF_ALGORITHM_H
