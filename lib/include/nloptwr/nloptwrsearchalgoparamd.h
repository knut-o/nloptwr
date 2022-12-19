
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
   */
  NLOptWrSearchAlgoParamD(nloptwr::SSTRAT strat, bool constr, bool grad,
                          size_t d);

  /**
   * constructor
   */
  NLOptWrSearchAlgoParamD(const NLOptWrSearchAlgoParam &alg, size_t d);

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
