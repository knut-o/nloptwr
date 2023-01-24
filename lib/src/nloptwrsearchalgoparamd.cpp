
#include "nloptwr/nloptwrsearchalgoparamd.h"
#include "nloptwr/nloptwrsearchalgoparam.h"

#include <iomanip>
#include <iostream> // std::cout
#include <sstream>  // std::stringstream
#include <string>

using namespace std;

namespace nloptwr {

NLOptWrSearchAlgoParamD::NLOptWrSearchAlgoParamD(nloptwr::SSTRAT strat,
                                                 bool hasEqconstr,
                                                 bool hasNeConstr, bool grad,
                                                 size_t d)
    : NLOptWrSearchAlgoParam(strat, hasEqconstr, hasNeConstr, grad), dim(d) {}

NLOptWrSearchAlgoParamD::NLOptWrSearchAlgoParamD(
    const NLOptWrSearchAlgoParam &alg, size_t d)
    : NLOptWrSearchAlgoParam(alg), dim(d) {}

NLOptWrSearchAlgoParamD::NLOptWrSearchAlgoParamD(
    nloptwr::SSTRAT strat, const NLOptWrSearchAlgoParamD &alg)
    : NLOptWrSearchAlgoParam(strat, alg), dim(alg.getDim()) {}

NLOptWrSearchAlgoParamD::~NLOptWrSearchAlgoParamD() {}

/// det dimension
size_t NLOptWrSearchAlgoParamD::getDim() const { return dim; }

std::string NLOptWrSearchAlgoParamD::toString() const {
  stringstream ss;

  ss << "NLOptWrSearchAlgoParamD( "
     << "searchStrategy=nloptwr::SSTRAT::" << left << setw(2)
     << getStrategyAsString(getSearchStrategy())
     << ", hasEqContraints=" << getHasEqContraints()
     << ", hasNeContraints=" << getHasNeContraints()
     << ", useGrad=" << getUseGradient() << ", dim=" << setw(3) << dim << " ) ";

  return ss.str();
}

std::string NLOptWrSearchAlgoParamD::toString2() const {
  stringstream ss;

  ss << "nloptwr::NLOptWrSearchAlgoParamD( "
     << "nloptwr::SSTRAT::" << left << setw(2)
     << getStrategyAsString(getSearchStrategy()) << ", "
     << (getHasEqContraints() ? "true " : "false") << ", "
     << (getHasNeContraints() ? "true " : "false") << ", "
     << (getUseGradient() ? "true " : "false") << ", " << right << setw(4)
     << dim << " ) ";

  return ss.str();
}

} // namespace nloptwr
