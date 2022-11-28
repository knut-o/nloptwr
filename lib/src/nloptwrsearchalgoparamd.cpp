
#include "nloptwr/nloptwrsearchalgoparamd.h"
#include "nloptwr/nloptwrsearchalgoparam.h"

#include <iomanip>
#include <iostream> // std::cout
#include <sstream>  // std::stringstream
#include <string>

using namespace std;

namespace nloptwr {

NLOptWrSearchAlgoParamD::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT strat,
                                                 bool constr, bool grad,
                                                 size_t d)
    : NLOptWrSearchAlgoParam(strat, constr, grad), dim(d) {}

NLOptWrSearchAlgoParamD::NLOptWrSearchAlgoParamD(
    const NLOptWrSearchAlgoParam &alg, size_t d)
    : NLOptWrSearchAlgoParam(alg), dim(d) {}

NLOptWrSearchAlgoParamD::~NLOptWrSearchAlgoParamD() {}

/// det dimension
size_t NLOptWrSearchAlgoParamD::getDim() const { return dim; }

std::string NLOptWrSearchAlgoParamD::toString() const {
  stringstream ss;

  ss << "NLOptWrSearchAlgoParam3( "
     << "searchStrategy=nloptwr::SSTRAT::" << left << setw(2)
     << getStrategyAsString(getSearchStrategy())
     << ", hasContraints=" << getHasContraints()
     << ", useGrad=" << getUseGradient() << ", dim=" << setw(3) << dim << " ) ";

  return ss.str();
}

std::string NLOptWrSearchAlgoParamD::toString2() const {
  stringstream ss;

  ss << "nloptwr::NLOptWrSearchAlgoParam3( "
     << "nloptwr::SSTRAT::" << left << setw(2)
     << getStrategyAsString(getSearchStrategy()) << ", "
     << (getHasContraints() ? "true " : "false") << ", "
     << (getUseGradient() ? "true " : "false") << ", " << right << setw(4)
     << dim << " ) ";

  return ss.str();
}

} // namespace nloptwr
