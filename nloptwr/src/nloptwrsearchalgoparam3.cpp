
#include "nloptwr/nloptwrsearchalgoparam3.h"
#include "nloptwr/nloptwrsearchalgoparam.h"
#include "nloptwr/nloptwrsearchalgoparam2.h"

#include <iomanip>
#include <iostream> // std::cout
#include <sstream>  // std::stringstream
#include <string>

using namespace std;

namespace nloptwr {

NLOptWrSearchAlgoParam3::NLOptWrSearchAlgoParam3(nloptwr::SSTRAT strat,
                                                 bool constr, bool grad,
                                                 bool uaugl, size_t d)
    : NLOptWrSearchAlgoParam2(strat, constr, grad, uaugl), dim(d) {}

NLOptWrSearchAlgoParam3::NLOptWrSearchAlgoParam3(
    const NLOptWrSearchAlgoParam2 &alg, size_t d)
    : NLOptWrSearchAlgoParam2(alg), dim(d) {}

NLOptWrSearchAlgoParam3::~NLOptWrSearchAlgoParam3() {}

/// det dimension
size_t NLOptWrSearchAlgoParam3::getDim() const { return dim; }

std::string NLOptWrSearchAlgoParam3::toString() const {
  stringstream ss;

  ss << "NLOptWrSearchAlgoParam3( "
     << "searchStrategy=nloptwr::SSTRAT::" << left << setw(2)
     << getStrategyAsString(getSearchStrategy())
     << ", hasContraints=" << getHasContraints()
     << ", useGrad=" << getUseGradient()
     << ", useAugLagBeforeMLSL=" << getUseAugLagBeforeMLSL()
     << ", dim=" << setw(3) << dim << " ) ";

  return ss.str();
}

// nloptwr::NLOptWrSearchAlgoParam3(nloptwr::G , false, false, true ,  3), {
// nlopt::GN_DIRECT } },
std::string NLOptWrSearchAlgoParam3::toString2() const {
  stringstream ss;

  ss << "nloptwr::NLOptWrSearchAlgoParam3( "
     << "nloptwr::SSTRAT::" << left << setw(2)
     << getStrategyAsString(getSearchStrategy()) << ", "
     << (getHasContraints() ? "true " : "false") << ", "
     << (getUseGradient() ? "true " : "false") << ", "
     << (getUseAugLagBeforeMLSL() ? "true " : "false") << ", " << right
     << setw(4) << dim << " ) ";

  return ss.str();
}

} // namespace nloptwr
