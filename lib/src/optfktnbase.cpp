
#include "nloptwr/optfktnbase.h"

namespace oif {

OptFknBase::OptFknBase() {}

// virtual
OptFknBase::~OptFknBase() {}

// get function object
optFunction OptFknBase::getOptFunktion() {
  // optFunction optFktn
  // myOptFunction =
  [&](const std::vector<double> &xVec1, std::vector<double> &neconstraints)
  // -> double
  {
    // return
    optFktn(xVec1, neconstraints);
  };

  // return optFktn;
  return myOptFunction;
};

/*
std::vector<double> OptFknBase::getZeroNeConstraintVec() {
  return std::vector<double>(getSizeOfX());
}
*/

} // namespace oif
