
#include "nloptwr/nloptwrsstrat.h"

#include <iomanip>
#include <iostream> // std::cout
#include <sstream>  // std::stringstream
#include <string>

using namespace std;

namespace nloptwr {

NLOptWrSStrat::NLOptWrSStrat(SSTRAT searchStrat, bool useGrad)
    : searchStrategy(searchStrat), useGradient(useGrad) {
  if (SSTRAT::R == searchStrat) {
    useGradient = false;
  }
}

NLOptWrSStrat::NLOptWrSStrat(const NLOptWrSStrat &src) {
  searchStrategy = src.searchStrategy;
  useGradient = src.useGradient;
};

// virtual
NLOptWrSStrat::~NLOptWrSStrat() {}

SSTRAT NLOptWrSStrat::getSearchStrategy() const { return searchStrategy; }

void NLOptWrSStrat::setSearchStrategy(SSTRAT val) { searchStrategy = val; }

bool NLOptWrSStrat::getUseGradient() const { return useGradient; }

std::string NLOptWrSStrat::toString() const {
  stringstream ss;

  ss << "NloptWrSStrat( "
     << "searchStrategy=" << left << setw(2)
     << getStrategyAsString(searchStrategy) << ", useGradient=" << useGradient
     << " ) ";

  return ss.str();
}

} // namespace nloptwr
