
#ifndef P4PREF_ALGORITHM3_H
#define P4PREF_ALGORITHM3_H

#include "nlopt.hpp"
#include "nloptwralgorithm.h"

#include <string>

#include "nloptwrsearchalgoparam.h"
#include "nloptwrsearchalgoparam2.h"

namespace nloptwr {

/**
 * Container of search parameters and dimension
 */
class NLOptWrSearchAlgoParam3 : public NLOptWrSearchAlgoParam2 {

public:
  /**
   * constructor
   */
  NLOptWrSearchAlgoParam3(nloptwr::SSTRAT strat, bool constr, bool grad, size_t d);

  /**
   * constructor
   */
  NLOptWrSearchAlgoParam3(const NLOptWrSearchAlgoParam2 &alg, size_t d);

  /// default constructor
  NLOptWrSearchAlgoParam3() = delete;

  /// destructor
  virtual ~NLOptWrSearchAlgoParam3();

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
