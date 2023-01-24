
#include "nloptwr/nloptwrsearchalgoparam.h"

#include <iomanip>
#include <iostream> // std::cout
#include <sstream>  // std::stringstream
#include <string>

using namespace std;

namespace nloptwr {

NLOptWrSearchAlgoParam::NLOptWrSearchAlgoParam(SSTRAT searchStrat,
                                               bool hasEqConstr,
                                               bool hasNeConstr, bool useGrad)
    : searchStrategy(searchStrat), hasEqContraints(hasEqConstr),
      hasNeContraints(hasNeConstr), useGradient(useGrad) {
  if (SSTRAT::R == searchStrat) {
    useGradient = false;
  }
}

NLOptWrSearchAlgoParam::NLOptWrSearchAlgoParam(
    SSTRAT searchStrat, const NLOptWrSearchAlgoParam &src)
    : searchStrategy(searchStrat), hasEqContraints(src.getHasEqContraints()),
      hasNeContraints(src.getHasNeContraints()),
      useGradient(src.getUseGradient()) {
  if (SSTRAT::R == searchStrat) {
    useGradient = false;
  }
}

NLOptWrSearchAlgoParam::NLOptWrSearchAlgoParam() {}

NLOptWrSearchAlgoParam::NLOptWrSearchAlgoParam(
    const NLOptWrSearchAlgoParam &src) {
  searchStrategy = src.searchStrategy;
  hasEqContraints = src.hasEqContraints;
  hasNeContraints = src.hasNeContraints;
  useGradient = src.useGradient;
};

NLOptWrSearchAlgoParam::~NLOptWrSearchAlgoParam() {}

bool NLOptWrSearchAlgoParam::operator<(
    const NLOptWrSearchAlgoParam &rhs) const {
  int r = 0;
  if (r == 0)
    if (searchStrategy != rhs.searchStrategy) {
      r = (searchStrategy < rhs.searchStrategy) ? 1 : -1;
    }
  if (r == 0)
    if (hasEqContraints != rhs.hasEqContraints) {
      r = (hasEqContraints) ? 1 : -1;
    }
  if (r == 0)
    if (hasNeContraints != rhs.hasNeContraints) {
      r = (hasNeContraints) ? 1 : -1;
    }
  if (r == 0)
    if (useGradient != rhs.useGradient) {
      r = (useGradient) ? 1 : -1;
    }

  bool result = (r < 0);

  return result;
}

SSTRAT NLOptWrSearchAlgoParam::getSearchStrategy() const {
  return searchStrategy;
}

void NLOptWrSearchAlgoParam::setSearchStrategy(SSTRAT val) {
  searchStrategy = val;
}

bool NLOptWrSearchAlgoParam::getHasContraints() const {
  return (hasEqContraints || hasNeContraints);
}

bool NLOptWrSearchAlgoParam::getHasEqContraints() const {
  return hasEqContraints;
}

bool NLOptWrSearchAlgoParam::getHasNeContraints() const {
  return hasNeContraints;
}

bool NLOptWrSearchAlgoParam::getUseGradient() const { return useGradient; }

void NLOptWrSearchAlgoParam::setUseEqContraints(bool val) {
  hasEqContraints = val;
}

void NLOptWrSearchAlgoParam::setUseNeContraints(bool val) {
  hasNeContraints = val;
}

std::string NLOptWrSearchAlgoParam::toString() const {
  stringstream ss;

  ss << "P4PrefAlgorithm( "
     << "searchStrategy=" << setw(2) << getStrategyAsString(searchStrategy)
     << ", hasEqContraints=" << hasEqContraints
     << ", hasNeContraints=" << hasNeContraints
     << ", useGradient=" << useGradient << " ) ";

  return ss.str();
}

std::string NLOptWrSearchAlgoParam::toStringShort() const {
  stringstream ss;

  ss << "P4PrefAlgorithm("
     << "Strat=" << left << setw(2) << getStrategyAsString(searchStrategy)
     << ", Ceq=" << hasEqContraints << ", Cne=" << hasNeContraints
     << ", Grad=" << useGradient << ") ";

  return ss.str();
}

} // namespace nloptwr
