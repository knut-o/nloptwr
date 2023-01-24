
#ifndef P4PREF_ALGORITHM3_H
#define P4PREF_ALGORITHM3_H

#include "nlopt.hpp"
#include "nloptwralgorithm.h"

#include <string>

#include "nloptwrsearchalgoparam.h"

namespace nloptwr {

/**
 * Container of search parameters and dimension
 */
class NLOptWrSearchAlgoParamD : public NLOptWrSearchAlgoParam {

public:
  /**
   * constructor
   * @param strat search strategy
   * @param hasEqconstr equality constraints flag
   * @param hasNeconstr nonequality constraints flag
   * @param grad has gradient flag
   * @param d dimension (number of parameters)
   */
  NLOptWrSearchAlgoParamD(nloptwr::SSTRAT strat, bool hasEqconstr,
                          bool hasNeConstr, bool grad, size_t d);

  /**
   * constructor
   * @param alg source object
   * @param dim number of arguments
   */
  NLOptWrSearchAlgoParamD(const NLOptWrSearchAlgoParam &alg, size_t d);

  /**
   * constructor
   * @param strat search strategy
   * @param source object
   */
  NLOptWrSearchAlgoParamD(nloptwr::SSTRAT strat,
                          const NLOptWrSearchAlgoParamD &alg);

  /// default constructor
  NLOptWrSearchAlgoParamD() = delete;

  /// destructor
  virtual ~NLOptWrSearchAlgoParamD();

  /// det dimension
  size_t getDim() const;

  /// get string representation
  std::string toString() const;

  /// get a copmputer readable string representation
  std::string toString2() const;

private:
  /// dimension
  size_t dim;
};
} // namespace nloptwr

#endif // P4PREF_ALGORITHM2_H
