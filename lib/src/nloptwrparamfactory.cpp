
#include "nloptwr/nloptwrparamfactory.h"
#include "nloptwr/nloptwralgorithm.h"

#include "nloptwr/nloptwrsearchalgoparam.h"
#include "nloptwr/nloptwrsearchalgoparamd.h"

#include "nloptwr/paramrequirement.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace nloptwr {

NLOptWrParamFactory::NLOptWrParamFactory() {
  for (size_t i = 0; i < nlOptAlgorithms.size(); ++i) {
    nlopt::algorithm alg = nlOptAlgorithms.at(i).getAlgorithmEnum();
    sStrategy2Idx[alg] = i;
  }
}

NLOptWrParamFactory::~NLOptWrParamFactory() {}

// ============================================================================

const std::string &
NLOptWrParamFactory::getNameOfAlgorithm(nlopt::algorithm alg) const {
  return getNameOfIdx(getIdxOfEnum(alg));
}

const std::string &NLOptWrParamFactory::getNameOfIdx(size_t idx) const {
  return nlOptAlgorithms[idx].getName();
}

size_t NLOptWrParamFactory::getIdxOfEnum(nlopt::algorithm alg) const {
  size_t result = 0;
  std::map<nlopt::algorithm, size_t>::const_iterator it =
      sStrategy2Idx.find(alg);
  if (it != sStrategy2Idx.end()) {
    result = (*it).second;
  }
  return result;
}

nlopt::algorithm NLOptWrParamFactory::getEnumOfIdx(size_t idx) const {
  return nlOptAlgorithms.at(idx).getAlgorithmEnum();
}

void NLOptWrParamFactory::setPreferedAlgorithm(nlopt::algorithm algx,
                                               int xDimMin, int xDimMax) {
  size_t iPos = getIdxOfEnum(algx);
  NLOptWrAlgorithm algx1(nlOptAlgorithms.at(iPos));

  if (xDimMin > 0)
    algx1.setMinParameters(xDimMin);

  if (xDimMax >= 0)
    algx1.setMaxParameters(xDimMax);

  if (preferedOptAlgorithms.size() >= preferedOptAlgorithms.capacity())
    preferedOptAlgorithms.reserve((1 + preferedOptAlgorithms.size()) * 2);

  if (find(preferedOptAlgorithms.begin(), preferedOptAlgorithms.end(), algx1) ==
      preferedOptAlgorithms.end()) {
    preferedOptAlgorithms.push_back(algx1);
  }
}

void NLOptWrParamFactory::setPreferedAlgorithms(
    const std::vector<nlopt::algorithm> &algxs) {
  for (size_t i = 0; i < algxs.size(); i++) {
    setPreferedAlgorithm(algxs.at(i));
  }
}

const std::vector<NLOptWrAlgorithm> &
NLOptWrParamFactory::getPreferedAlgorithms() const {
  return preferedOptAlgorithms;
}

bool NLOptWrParamFactory::hasPreferedAlgorithms() const {
  return (!preferedOptAlgorithms.empty());
}

void NLOptWrParamFactory::deletePreferedAlgorithms() {
  preferedOptAlgorithms.clear();
}

// ============================================================================

vector<NLOptWrAlgorithm> NLOptWrParamFactory::getAlgorithm(
    const NLOptWrSearchAlgoParamD &p4pAlg0) const {

  vector<NLOptWrAlgorithm> result;
  vector<NLOptWrAlgorithm> result1;
  vector<NLOptWrAlgorithm> result2;

  NLOptWrSearchAlgoParamD p4pAlg(p4pAlg0);

  // local optimizations without constraints do not need meta algorithms at all
  if (p4pAlg0.getSearchStrategy() == SSTRAT::LM &&
      !p4pAlg0.getHasContraints()) {
    p4pAlg = NLOptWrSearchAlgoParamD(SSTRAT::L, p4pAlg);
  }

  // global optimizations without constraints do not need meta algorithms at all
  if (p4pAlg0.getSearchStrategy() == SSTRAT::GM &&
      !p4pAlg0.getHasContraints()) {
    p4pAlg = NLOptWrSearchAlgoParamD(SSTRAT::G, p4pAlg);
  }

  SSTRAT req4SStrat = p4pAlg.getSearchStrategy();

  // meta search strategies:
  if (isMetaSearchStrategy(req4SStrat)) {
    if (p4pAlg.getHasContraints()) {
      if (req4SStrat == SSTRAT::LM || req4SStrat == SSTRAT::GM) {
        if (p4pAlg.getHasEqContraints()) {
          result1 = getAvailableAlgorithms(A_ENUM::A_AUGLAG_EQ, p4pAlg);
          result2 = getAvailableAlgorithms(A_ENUM::A_AUGLAG_EQ_SUB, p4pAlg);
        } else {
          result1 = getAvailableAlgorithms(A_ENUM::A_AUGLAG_NE, p4pAlg);
          result2 = getAvailableAlgorithms(A_ENUM::A_AUGLAG_NE_SUB, p4pAlg);
        }
      }
    } else {
      if (req4SStrat == SSTRAT::LM || req4SStrat == SSTRAT::GM) {
        result1 = getAvailableAlgorithms(A_ENUM::A_MLSL, p4pAlg);
        result2 = getAvailableAlgorithms(A_ENUM::A_MLSL_SUB, p4pAlg);
      }
    }

    result = getResultAlgorithms(result1, result2, true);
  } else {
    // non-meta search strategies:

    if (req4SStrat == SSTRAT::L) {
      result1 = getAvailableAlgorithms(A_ENUM::A_L, p4pAlg);
    } else if (req4SStrat == SSTRAT::G) {
      result1 = getAvailableAlgorithms(A_ENUM::A_G, p4pAlg);
    } else if (req4SStrat == SSTRAT::R) {
      result1 = getAvailableAlgorithms(A_ENUM::A_R, p4pAlg);
    }
    result = getResultAlgorithms(result1, result2, false);
  }

  // fallback
  if (result.empty()) {
    p4pAlg = NLOptWrSearchAlgoParamD(SSTRAT::R, p4pAlg);
    result1 = getAvailableAlgorithms(A_ENUM::A_R, p4pAlg);
    result = getResultAlgorithms(result1, result2, false);
  }

  return result;
}

// ============================================================================

vector<NLOptWrAlgorithm> NLOptWrParamFactory::getAvailableAlgorithms(
    A_ENUM algE, const NLOptWrSearchAlgoParamD &p4pAlg) const {

  vector<NLOptWrAlgorithm> result1;
  vector<NLOptWrAlgorithm> result2;

  result1 = getAvailableAlgorithms(algE, p4pAlg, preferedOptAlgorithms);
  result2 = getAvailableAlgorithms(algE, p4pAlg, nlOptAlgorithms);

  vector<NLOptWrAlgorithm> result;
  size_t resultSize = (result1.size() + result2.size());
  if (result.capacity() < resultSize)
    result.reserve(resultSize);

  for (auto r : result1) {
    result.push_back(r);
  }
  for (auto r : result2) {
    result.push_back(r);
  }

  return result;
}

// ============================================================================

vector<NLOptWrAlgorithm> NLOptWrParamFactory::getAvailableAlgorithms(
    A_ENUM algE, const NLOptWrSearchAlgoParamD &p4pAlg,
    const vector<class NLOptWrAlgorithm> &availableAlgs) const {
  vector<NLOptWrAlgorithm> results;

  size_t xDim = p4pAlg.getDim();

  if (results.capacity() <= availableAlgs.size())
    results.reserve(availableAlgs.size() + 1);

  // enum(algorithm), name(algorithm), STRAT, GRAD,EC, NEC, nSUB, Min, Max
  SSTRAT req4SearchStrategy = p4pAlg.getSearchStrategy();

  bool regAugLag = false;
  bool reqMLSL = false;

  ParamRequirement reqHasEqConstraints, reqHasNeConstraints;

  reqHasEqConstraints.setReqired(p4pAlg.getHasEqContraints());
  reqHasNeConstraints.setReqired(p4pAlg.getHasNeContraints());

  bool reqUseGradient = p4pAlg.getUseGradient();
  bool reqNeedsSubOpt = false;

  if (isMetaSearchStrategy(req4SearchStrategy)) {
    reqNeedsSubOpt = true;
    switch (algE) {
    case A_AUGLAG_EQ:
      regAugLag = true;
      reqHasEqConstraints.setReqired(true);
      reqHasNeConstraints.setReqired(false);
      break;
    case A_AUGLAG_EQ_SUB:
      reqHasEqConstraints.setReqired(false);
      reqNeedsSubOpt = false;
      // req4SearchStrategy = SSTRAT::L;
      req4SearchStrategy = getNonMetaSearchStrategy(req4SearchStrategy);
      break;

    // ====================================
    case A_AUGLAG_NE:
      regAugLag = true;
      reqHasEqConstraints.setReqired(false);
      reqHasNeConstraints.setReqired(true);
      break;
    case A_AUGLAG_NE_SUB:
      reqHasNeConstraints.setReqired(false);
      reqNeedsSubOpt = false;
      // req4SearchStrategy = SSTRAT::L;
      req4SearchStrategy = getNonMetaSearchStrategy(req4SearchStrategy);
      break;
    // ====================================
    case A_MLSL:
      reqMLSL = true;
      reqHasEqConstraints.setReqired(false);
      reqHasNeConstraints.setReqired(false);
      break;
    case A_MLSL_SUB:
      reqNeedsSubOpt = false;
      req4SearchStrategy = SSTRAT::L;
      reqHasEqConstraints.setReqired(false);
      reqHasNeConstraints.setReqired(false);
      break;
    // ====================================
    default:
      break;
    };
  } else if (isSearchStrategyR(req4SearchStrategy)) {
    reqUseGradient = false;
  }

  bool isWrongCombination1 =
      ((A_AUGLAG_NE == algE || A_AUGLAG_NE_SUB == algE) &&
       p4pAlg.getHasEqContraints());
  bool isWrongCombination2 =
      ((A_MLSL == algE) &&
       (p4pAlg.getHasEqContraints() || p4pAlg.getHasNeContraints()));
  bool isWrongCombination = (isWrongCombination1 || isWrongCombination2);

  if (!isWrongCombination) {
    // search loop
    for (size_t i = 0; i < availableAlgs.size(); i++) {
      const NLOptWrAlgorithm &alg = availableAlgs.at(i);

      bool isSearchStrategyOK =
          (regAugLag && alg.isAugLag()) || (reqMLSL && alg.isMLSL());
      if (!isSearchStrategyOK)
        isSearchStrategyOK = (req4SearchStrategy == alg.getSearchStrategy());

      bool isEqConstraintsOK =
          reqHasEqConstraints.isFullfilled(alg.canHandleEqualConstraints());
      bool isNeConstraintsOK =
          reqHasNeConstraints.isFullfilled(alg.canHandleNonEqualConstraints());
      bool isGradOK = (reqUseGradient == alg.getUseGradient());
      bool isSubOptFlagOK = (reqNeedsSubOpt == alg.getNeedsSubopt());
      bool isInRangeOK =
          ((alg.getMaxParameters() == 0) || (xDim <= alg.getMaxParameters()));

      if (isSearchStrategyOK && isEqConstraintsOK && isNeConstraintsOK &&
          isGradOK && isSubOptFlagOK && isInRangeOK) {
        results.push_back(alg);
      }
    }
  }

  return results;
}

// ============================================================================

vector<NLOptWrAlgorithm>
NLOptWrParamFactory::getResultAlgorithms(const vector<NLOptWrAlgorithm> &algs1,
                                         const vector<NLOptWrAlgorithm> &algs2,
                                         bool hasSubAlg) {
  vector<NLOptWrAlgorithm> result;

  bool hasResult = (!algs1.empty());
  if (hasSubAlg)
    hasResult &= (!algs2.empty());

  if (hasResult) {
    size_t resultSize = (hasSubAlg) ? 2 : 1;
    if (result.capacity() < resultSize)
      result.reserve(resultSize);

    result.push_back(algs1[0]);

    if (hasSubAlg) {
      result.push_back(algs2[0]);
    }
  }

  return result;
}

// ============================================================================

bool NLOptWrParamFactory::isAuglag(nlopt::algorithm alg) const {
  return (getIdxOfEnum(alg));
}

bool NLOptWrParamFactory::isAuglag(std::size_t idx) const {
  return nlOptAlgorithms.at(idx).isAugLag();
}

bool NLOptWrParamFactory::isMLSL(nlopt::algorithm alg) const {
  return (getIdxOfEnum(alg));
}

bool NLOptWrParamFactory::isMLSL(std::size_t idx) const {
  return nlOptAlgorithms.at(idx).isMLSL();
}

// ============================================================================

// static
const std::vector<class NLOptWrAlgorithm> NLOptWrParamFactory::nlOptAlgorithms{
    // enum(algorithm), name(algorithm), STRAT, GRAD,EC, NEC, nSUB, Min, Max
    {nlopt::LN_AUGLAG_EQ, "LN_AUGLAG_EQ", SSTRAT::LM, false, true, false, true,
     1, 0},
    {nlopt::LD_AUGLAG_EQ, "LD_AUGLAG_EQ", SSTRAT::LM, true, true, false, true,
     1, 0},
    {nlopt::AUGLAG_EQ, "AUGLAG_EQ", SSTRAT::LM, true, true, false, true, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LD_AUGLAG, "LD_AUGLAG", SSTRAT::LM, true, false, true, true, 1, 0},
    {nlopt::LN_AUGLAG, "LN_AUGLAG", SSTRAT::LM, false, false, true, true, 1, 0},
    {nlopt::AUGLAG, "AUGLAG", SSTRAT::LM, true, false, true, true, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GD_MLSL, "GD_MLSL", SSTRAT::GM, true, false, false, true, 1, 0},
    {nlopt::GN_MLSL, "GN_MLSL", SSTRAT::GM, false, false, false, true, 1, 0},
    {nlopt::GD_MLSL_LDS, "GD_MLSL_LDS", SSTRAT::GM, true, false, false, true, 1,
     0},
    {nlopt::GN_MLSL_LDS, "GN_MLSL_LDS", SSTRAT::GM, false, false, false, true,
     1, 0},
    {nlopt::G_MLSL, "G_MLSL", SSTRAT::GM, true, false, false, true, 1, 0},
    {nlopt::G_MLSL_LDS, "G_MLSL_LDS", SSTRAT::GM, true, false, false, true, 1,
     0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LD_LBFGS, "LD_LBFGS", SSTRAT::L, true, false, false, false, 1, 0},
    {nlopt::LD_LBFGS_NOCEDAL, "LD_LBFGS_NOCEDAL", SSTRAT::L, true, false, false,
     false, 1, 0},
    {nlopt::LD_TNEWTON, "LD_TNEWTON", SSTRAT::L, true, false, false, false, 1,
     0},
    {nlopt::LD_TNEWTON_PRECOND, "LD_TNEWTON_PRECOND", SSTRAT::L, true, false,
     false, false, 1, 0},

    {nlopt::LD_TNEWTON_PRECOND_RESTART, "LD_TNEWTON_PRECOND_RESTART", SSTRAT::L,
     true, false, false, false, 1, 0},

    {nlopt::LD_TNEWTON_RESTART, "LD_TNEWTON_RESTART", SSTRAT::L, true, false,
     false, false, 1, 0},

    {nlopt::LD_VAR1, "LD_VAR1", SSTRAT::L, true, false, false, false, 1, 0},
    {nlopt::LD_VAR2, "LD_VAR2", SSTRAT::L, true, false, false, false, 1, 0},

    // ----------------------------------------------------------------------------------------------------------
    {nlopt::LD_MMA, "LD_MMA", SSTRAT::L, true, false, true, false, 1, 0},
    {nlopt::LD_CCSAQ, "LD_CCSAQ", SSTRAT::L, true, false, true, false, 1, 800},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LD_SLSQP, "LD_SLSQP", SSTRAT::L, true, true, true, false, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LN_COBYLA, "LN_COBYLA", SSTRAT::L, false, true, true, false, 1, 15},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LN_SBPLX, "LN_SBPLX", SSTRAT::L, false, false, false, false, 1, 15},
    {nlopt::LN_BOBYQA, "LN_BOBYQA", SSTRAT::L, false, false, false, false, 1,
     12},
    {nlopt::LN_NELDERMEAD, "LN_NELDERMEAD", SSTRAT::L, false, false, false,
     false, 1, 12},

    {nlopt::LN_NEWUOA, "LN_NEWUOA", SSTRAT::L, false, false, false, false, 1,
     12},
    {nlopt::LN_NEWUOA_BOUND, "LN_NEWUOA_BOUND", SSTRAT::L, false, false, false,
     false, 1, 12},
    {nlopt::LN_PRAXIS, "LN_PRAXIS", SSTRAT::L, false, false, false, false, 1,
     12},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GN_DIRECT, "GN_DIRECT", SSTRAT::G, false, false, false, false, 1,
     15},
    {nlopt::GN_DIRECT_L, "GN_DIRECT_L", SSTRAT::G, false, false, false, false,
     1, 15},
    {nlopt::GN_DIRECT_L_NOSCAL, "GN_DIRECT_L_NOSCAL", SSTRAT::G, false, false,
     false, false, 1, 15},

    {nlopt::GN_DIRECT_L_RAND, "GN_DIRECT_L_RAND", SSTRAT::G, false, false,
     false, false, 1, 15},

    {nlopt::GN_DIRECT_L_RAND_NOSCAL, "GN_DIRECT_L_RAND_NOSCAL", SSTRAT::G,
     false, false, false, false, 1, 0},

    {nlopt::GN_DIRECT_NOSCAL, "GN_DIRECT_NOSCAL", SSTRAT::G, false, false,
     false, false, 1, 15},

    {nlopt::GN_ORIG_DIRECT, "GN_ORIG_DIRECT", SSTRAT::G, false, false, false,
     false, 1, 15},

    {nlopt::GN_ORIG_DIRECT_L, "GN_ORIG_DIRECT_L", SSTRAT::G, false, false,
     false, false, 1, 0},

    {nlopt::GN_AGS, "GN_AGS", SSTRAT::G, false, false, false, false, 1, 5},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GD_STOGO, "GD_STOGO", SSTRAT::G, true, false, false, false, 1, 15},
    {nlopt::GD_STOGO_RAND, "GD_STOGO_RAND", SSTRAT::G, true, false, false,
     false, 1, 20},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GN_CRS2_LM, "GN_CRS2_LM", SSTRAT::R, false, false, false, false, 1,
     0},
    {nlopt::GN_ESCH, "GN_ESCH", SSTRAT::R, false, false, false, false, 1, 5},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GN_ISRES, "GN_ISRES", SSTRAT::R, false, true, true, false, 1, 0}

    // ----------------------------------------------------------------------------------------------------------
};

} // namespace nloptwr
