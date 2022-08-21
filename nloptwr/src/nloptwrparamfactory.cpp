
#include "nloptwr/nloptwrparamfactory.h"
#include "nloptwr/nloptwralgorithm.h"

#include "nloptwr/nloptwrsearchalgoparam.h"
#include "nloptwr/nloptwrsearchalgoparam2.h"
#include "nloptwr/nloptwrsearchalgoparam3.h"

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

// ==================================================================================================
// ==================================================================================================
// ==================================================================================================
// ==================================================================================================

vector<NLOptWrAlgorithm>
NLOptWrParamFactory::getAlgorithm(SSTRAT searchStrategy, bool hasConstraints,
                                  bool useGradient, bool useAugLagBeforeMlsl,
                                  std::size_t xDim) const {
  NLOptWrSearchAlgoParam3 p4pAlg3(searchStrategy, hasConstraints, useGradient,
                                  useAugLagBeforeMlsl, xDim);
  return getAlgorithm(p4pAlg3);
}

vector<NLOptWrAlgorithm>
NLOptWrParamFactory::getAlgorithm(const NLOptWrSearchAlgoParam2 &p4pAlg,
                                  std::size_t xDim) const {
  NLOptWrSearchAlgoParam3 p4pAlg3(p4pAlg, xDim);

  return getAlgorithm(p4pAlg3);
}

vector<NLOptWrAlgorithm> NLOptWrParamFactory::getAlgorithm(
    const NLOptWrSearchAlgoParam3 &p4pAlg0) const {
  // if (d) cout << "DEBUG: NlOptParamFactory::getAlgorithm[-1]: " <<
  // p4pAlg.toString() << endl;

  NLOptWrSearchAlgoParam3 p4pAlg(p4pAlg0);
  // local optimizations without constraints do not need meta algorithms at all
  if (p4pAlg0.getSearchStrategy() == SSTRAT::LM &&
      !p4pAlg0.getHasContraints()) {
    p4pAlg = NLOptWrSearchAlgoParam3(
        SSTRAT::L, p4pAlg0.getHasContraints(), p4pAlg0.getUseGradient(),
        p4pAlg0.getUseAugLagBeforeMLSL(), p4pAlg0.getDim());
  }

  bool foundAlgorithm = false;
  bool useAugLagBeforeMLSL = p4pAlg.getUseAugLagBeforeMLSL();
  std::size_t xDim = p4pAlg.getDim();

  vector<NLOptWrAlgorithm> result;
  if (result.capacity() < 2)
    result.reserve(2);

  SSTRAT searchStrategy = p4pAlg.getSearchStrategy();
  bool hasConstraints = p4pAlg.getHasContraints();

  string forbiddenAlgorith;
  SSTRAT mainStrategy = searchStrategy;

  NLOptWrSearchAlgoParam p4pAlg1(p4pAlg);

  SSTRAT mainStrategy2(searchStrategy);
  int searchStrategyInt = static_cast<int>(searchStrategy);

  // exit in case of error
  if (searchStrategyInt == 0) {
    cerr << "ERROR: (searchStrategyInt==0) is not supported. => exit" << endl;
    cerr.flush();
    exit(1);
  }

  if (d && !foundAlgorithm) {
    cout << "DEBUG: NlOptParamFactory::getAlgorithm[00]: " << p4pAlg.toString()
         << ", searchStrategy=" << static_cast<int>(searchStrategy)
         << ",  mainStrategy2=" << static_cast<int>(mainStrategy2) << endl;
    cout << "DEBUG: NlOptParamFactory::getAlgorithm[01]: " << p4pAlg1.toString()
         << ", searchStrategy=" << static_cast<int>(searchStrategy)
         << ",  mainStrategy2=" << static_cast<int>(mainStrategy2) << endl;
  }

  if (!foundAlgorithm)
    foundAlgorithm = findSearchAlgorithmR(p4pAlg, xDim, result);

  if (!foundAlgorithm && isSearchStrategyLG(mainStrategy)) {
    if (d > 4)
      cout << "DEBUG: NlOptParamFactory::getAlgorithm[LG]: "
           << p4pAlg.toString()
           << ", searchStrategy=" << static_cast<int>(searchStrategy)
           << ",  mainStrategy2=" << static_cast<int>(mainStrategy2) << endl;
    // NOTE: case of no constraints: add Parameter: hasConstraints
    foundAlgorithm = findSearchAlgorithmLG(
        p4pAlg, xDim, hasConstraints,
        result); // TODO: case of no constraints: add Parameter: hasConstraints
    if (!foundAlgorithm) {
      mainStrategy2 = getMetaSearchStrategy(mainStrategy);
      p4pAlg1.setSearchStrategy(mainStrategy2);
      if (d > 4)
        cout << "DEBUG: NlOptParamFactory::getAlgorithm[M0]: "
             << p4pAlg.toString()
             << ", searchStrategy=" << static_cast<int>(searchStrategy)
             << ",  mainStrategy2=" << static_cast<int>(mainStrategy2) << endl;
    }
  }

  if (!foundAlgorithm && isMetaSearchStrategy(mainStrategy2)) {
    if (!foundAlgorithm && !hasConstraints && !useAugLagBeforeMLSL) {
      if (d > 4)
        cout << "DEBUG: NlOptParamFactory::getAlgorithm[M0]: "
             << p4pAlg.toString()
             << ", searchStrategy=" << static_cast<int>(searchStrategy)
             << ",  mainStrategy2=" << static_cast<int>(mainStrategy2) << endl;
      foundAlgorithm =
          findSearchAlgorithmMlsl(p4pAlg1, xDim, hasConstraints, result);
    }

    if (!foundAlgorithm && hasConstraints) {
      if (d > 4)
        cout << "DEBUG: NlOptParamFactory::getAlgorithm[MA]: "
             << p4pAlg.toString()
             << ", searchStrategy=" << static_cast<int>(searchStrategy)
             << ",  mainStrategy2=" << static_cast<int>(mainStrategy2) << endl;
      foundAlgorithm = findSearchAlgorithmAugLag(p4pAlg1, xDim, result);
    }

    if (!foundAlgorithm) {
      if (d > 4)
        cout << "DEBUG: NlOptParamFactory::getAlgorithm[M2]: "
             << p4pAlg.toString()
             << ", searchStrategy=" << static_cast<int>(searchStrategy)
             << ",  mainStrategy2=" << static_cast<int>(mainStrategy2) << endl;
      foundAlgorithm =
          findSearchAlgorithmMlsl(p4pAlg1, xDim, hasConstraints, result);
    }
  }

  // TODO: Fallback: Stochasctic search al last!!!
  if (!foundAlgorithm) {
    NLOptWrSearchAlgoParam2 p4pAlgR(SSTRAT::R, false, false);
    if (d > 4)
      cout << "DEBUG: NlOptParamFactory::getAlgorithm[RX]: "
           << p4pAlgR.toString() << "// Fallback to R" << endl;
    foundAlgorithm = findSearchAlgorithmAugLag(p4pAlgR, xDim, result);
  }

  return result;
}

// ==================================================================================================
// ==================================================================================================

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

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

bool NLOptWrParamFactory::findSearchAlgorithmR(
    const NLOptWrSearchAlgoParam &p4pAlg1, std::size_t xDim,
    vector<NLOptWrAlgorithm> &res) const {
  bool rc = false;
  res.clear();
  if (res.capacity() < 2)
    res.reserve(2);

  SSTRAT searcStrategyMain = p4pAlg1.getSearchStrategy();

  if ((!isSearchStrategyR(searcStrategyMain)))
    return rc;

  vector<NLOptWrAlgorithm> res1;
  NLOptWrSearchAlgoParam p4pAlg1Sub(p4pAlg1);

  bool ignoreConstraints = false;
  bool searchLocalOrGlobal = false;
  bool found1 = findSearchAlgorithm(p4pAlg1Sub, xDim, ignoreConstraints,
                                    searchLocalOrGlobal, true, res1);

  rc = (found1);
  if (rc) {
    res.push_back(res1[0]);
  }

  return rc;
}

// ==================================================================================================
// ==================================================================================================

bool NLOptWrParamFactory::findSearchAlgorithmMlsl(
    const NLOptWrSearchAlgoParam &p4pAlg1, std::size_t xDim,
    bool hasConstraints, vector<NLOptWrAlgorithm> &res) const {
  bool rc = false;
  res.clear();
  if (res.capacity() < 2)
    res.reserve(2);

  SSTRAT searcStrategyMain = p4pAlg1.getSearchStrategy();

  if (!isSearchStrategyLGM(searcStrategyMain))
    return rc;

  vector<NLOptWrAlgorithm> res1;
  // bool found1=findAugLag(res1);

  bool ignoreConstraints0 = true;
  bool searchLocalOrGlobal0 = false;
  bool isNotMLSL0 = false;
  bool found1 = findSearchAlgorithm(p4pAlg1, xDim, ignoreConstraints0,
                                    searchLocalOrGlobal0, isNotMLSL0, res1);

  NLOptWrSearchAlgoParam p4pAlg1Sub(p4pAlg1);
  p4pAlg1Sub.setSearchStrategy(getNonMetaSearchStrategy(searcStrategyMain));
  vector<NLOptWrAlgorithm> res2;
  bool ignoreConstraints = ((hasConstraints) ? false : true);
  bool searchLocalOrGlobal = true;
  bool isNotMLSL = false;

  bool found2 = findSearchAlgorithm(p4pAlg1Sub, xDim, ignoreConstraints,
                                    searchLocalOrGlobal, isNotMLSL, res2);

  rc = (found1 && found2);
  if (rc) {
    res.push_back(res1[0]);
    res.push_back(res2[0]);
  }

  return rc;
}

// ==================================================================================================
// ==================================================================================================

bool NLOptWrParamFactory::findSearchAlgorithmAugLag(
    const NLOptWrSearchAlgoParam &p4pAlg1, std::size_t xDim,
    vector<NLOptWrAlgorithm> &res) const {
  bool rc = false;
  res.clear();
  if (res.capacity() < 2)
    res.reserve(2);

  SSTRAT searcStrategyMain = p4pAlg1.getSearchStrategy();

  if (d > 6)
    cout << "DEBUG: NlOptParamFactory::findSearchAlgorithmAugLag [A]: p4pAlg1: "
         << p4pAlg1.toString() << endl;

  if ((!isSearchStrategyLGM(searcStrategyMain)) ||
      (!p4pAlg1.getHasContraints()))
    return rc;

  if (d > 6)
    cout << "DEBUG: NlOptParamFactory::findSearchAlgorithmAugLag [B]: p4pAlg1: "
         << p4pAlg1.toString() << endl;

  vector<NLOptWrAlgorithm> res1;
  // bool found1=findAugLag(res1);

  bool ignoreConstraints0 = false;
  bool searchLocalOrGlobal0 = false;
  bool isNotMLSL0 = true;
  bool found1 = findSearchAlgorithm(p4pAlg1, xDim, ignoreConstraints0,
                                    searchLocalOrGlobal0, isNotMLSL0, res1);

  if (d > 6) {
    if (!found1)
      cout
          << "DEBUG NlOptParamFactory::findSearchAlgorithmAugLag [C]: p4pAlg1: "
          << p4pAlg1.toString() << endl;
    cout << "DEBUG: NlOptParamFactory::findSearchAlgorithmAugLag [C]: "
         << ((!res.empty()) ? res1[0].getName() : "no result ") << endl;
  }

  NLOptWrSearchAlgoParam p4pAlg1Sub(p4pAlg1);

  p4pAlg1Sub.setSearchStrategy(getNonMetaSearchStrategy(searcStrategyMain));
  vector<NLOptWrAlgorithm> res2;

  bool ignoreConstraints = true;
  bool searchLocalOrGlobal = false;
  bool isNotMLSL = true;
  bool found2 = false;

  if (d > 6)
    cout << "DEBUG: NlOptParamFactory::findSearchAlgorithmAugLag [D]: "
            "p4pAlg1Sub: "
         << p4pAlg1Sub.toString() << endl;

  if (p4pAlg1Sub.getHasContraints()) {
    found2 = findSearchAlgorithm(p4pAlg1Sub, xDim, ignoreConstraints,
                                 searchLocalOrGlobal, isNotMLSL, res2);
  }

  rc = (found1 && found2);
  if (rc) {
    res.push_back(res1[0]);
    res.push_back(res2[0]);
  }

  if (d > 6)
    cout << "DEBUG: NlOptParamFactory::findSearchAlgorithmAugLag [E]: rc=" << rc
         << endl;

  return rc;
}

// ==================================================================================================
// ==================================================================================================

/// TODO: Don't ignore algorithms when no constraints exists!!!!
bool NLOptWrParamFactory::findSearchAlgorithmLG(
    const NLOptWrSearchAlgoParam &p4pAlg1, std::size_t xDim,
    bool hasConstraints, vector<NLOptWrAlgorithm> &res) const {
  bool rc = false;
  res.clear();
  if (res.capacity() < 2)
    res.reserve(2);

  SSTRAT searcStrategyMain = p4pAlg1.getSearchStrategy();

  if ((!isSearchStrategyLGM(searcStrategyMain)))
    return rc;

  vector<NLOptWrAlgorithm> res1;

  NLOptWrSearchAlgoParam p4pAlg1Sub(p4pAlg1);
  p4pAlg1Sub.setSearchStrategy(getNonMetaSearchStrategy(searcStrategyMain));

  bool ignoreConstraints = ((hasConstraints) ? false : true);
  bool searchLocalOrGlobal = false;
  bool isNotMLSL = true;

  // TODO: Do not exclude constraint capable algorithms!!!!
  rc = findSearchAlgorithm(p4pAlg1Sub, xDim, ignoreConstraints,
                           searchLocalOrGlobal, isNotMLSL, res1);

  if (rc)
    res.push_back(res1[0]);

  return rc;
}

// ==================================================================================================
// ==================================================================================================
// --------------------------------------------------------------------------------------------------

bool NLOptWrParamFactory::findSearchAlgorithm(
    const NLOptWrSearchAlgoParam &p4pAlg1, std::size_t xDim,
    bool ignoreConstraints, bool searchLocalOrGlobal, bool isNotMLSL,
    vector<NLOptWrAlgorithm> &res) const {
  bool result = false;
  res.clear();
  if (res.capacity() < 1)
    res.reserve(1);

  vector<NLOptWrAlgorithm> algTmp2 = getAvailableAlgorithms(
      p4pAlg1, xDim, ignoreConstraints, searchLocalOrGlobal, isNotMLSL);
  if (!algTmp2.empty()) {
    result = true;
    res.push_back(algTmp2[0]);
  }

  return result;
}

// --------------------------------------------------------------------------------------------------

// NOTE: next parameter : forbid-sublopt
vector<NLOptWrAlgorithm> NLOptWrParamFactory::getAvailableAlgorithms(
    const NLOptWrSearchAlgoParam &p4pAlg, std::size_t xDim,
    bool ignoreConstraints, bool searchLocalOrGlobal, bool isNotMLSL) const {
  vector<NLOptWrAlgorithm> rc;
  vector<vector<NLOptWrAlgorithm>> rcTmp(2);

  rcTmp[0] =
      getAvailableAlgorithms(p4pAlg, preferedOptAlgorithms, xDim,
                             ignoreConstraints, searchLocalOrGlobal, isNotMLSL);

  if (d > 10 && !rcTmp[0].empty()) {
    cout << "DEBUG: NlOptParamFactory::getAvailableAlgorithms: p4pAlg="
         << p4pAlg.toString() << ", xDim=" << xDim
         << ", ignoreConstraints=" << ignoreConstraints
         << ", searchLocalOrGlobal=" << searchLocalOrGlobal
         << ", isNotMLSL=" << isNotMLSL
         << " ==> rcTmp[0]=" << ((rcTmp.at(0)).at(0)).getName() << endl;
  }

  if (d > 9 && rcTmp[0].empty()) {
    cout << "DEBUG: NlOptParamFactory::getAvailableAlgorithms: p4pAlg="
         << p4pAlg.toString() << ", xDim=" << xDim
         << ", ignoreConstraints=" << ignoreConstraints
         << ", searchLocalOrGlobal=" << searchLocalOrGlobal
         << ", isNotMLSL=" << isNotMLSL << " ==> rcTmp[0]: NO RESULT " << endl;
  }

  rcTmp[1] =
      getAvailableAlgorithms(p4pAlg, nlOptAlgorithms, xDim, ignoreConstraints,
                             searchLocalOrGlobal, isNotMLSL);

  if (d > 10 && !rcTmp[1].empty()) {
    cout << "DEBUG: NlOptParamFactory::getAvailableAlgorithms: p4pAlg="
         << p4pAlg.toString() << ", xDim=" << xDim
         << ", ignoreConstraints=" << ignoreConstraints
         << ", searchLocalOrGlobal=" << searchLocalOrGlobal
         << ", isNotMLSL=" << isNotMLSL
         << " ==> rcTmp[1]=" << ((rcTmp.at(1)).at(0)).getName() << endl;
  }

  rc.reserve(rcTmp[0].size() + rcTmp[1].size());
  for (size_t i = 0; i < rcTmp.size(); i++) {
    for (size_t j = 0; j < rcTmp[i].size(); j++) {
      if (find(rc.begin(), rc.end(), rcTmp[i][j]) == rc.end()) {
        rc.push_back(rcTmp[i][j]);
      }
    }
  }

  if (d > 10)
    cout << "DEBUG: NlOptParamFactory::getAvailableAlgorithms: p4pAlg="
         << p4pAlg.toString() << ", xDim=" << xDim
         << ", ignoreConstraints=" << ignoreConstraints
         << ", searchLocalOrGlobal=" << searchLocalOrGlobal
         << ", isNotMLSL=" << isNotMLSL << ", rc.size()=" << rc.size()
         << " ==> rc[0]=" << ((rc.empty()) ? "" : rc[0].getName())
         << " = rc.size()=" << rc.size() << endl;

  return rc;
}

// NOTE next parameter : forbid-sublopt
vector<NLOptWrAlgorithm> NLOptWrParamFactory::getAvailableAlgorithms(
    const NLOptWrSearchAlgoParam &p4pAlg,
    const std::vector<class NLOptWrAlgorithm> &availableAlgs, std::size_t xDim,
    bool ignoreConstraints, bool searchLocalOrGlobal, bool isNotMLSL) const {
  vector<NLOptWrAlgorithm> results;
  if (results.capacity() <= availableAlgs.size())
    results.reserve(availableAlgs.size() + 1);

  SSTRAT p4SearchStrategy = p4pAlg.getSearchStrategy();
  bool p4UseGradient = p4pAlg.getUseGradient();
  bool p4HasConstraints = p4pAlg.getHasContraints();
  // bool p4SubOpt=isMetaSearchStrategy(p4SearchStrategy);

  // int  searchStrategyIntAbs=abs(static_cast<int>(searchStrategy));
  bool p4LocalOrGlobalSearch =
      isP4pAlgLocalOrGlobal(p4SearchStrategy, searchLocalOrGlobal);

  // int  algSearchStrategyInt =0;
  bool algStrategySearchLocalOrGlobal = false;

  // NOTE: NEW
  bool p4MLSL = false;
  bool p4AugLag = false;
  bool p4IsMetaAlgorith = isMetaSearchStrategy(p4SearchStrategy);
  if (p4IsMetaAlgorith) {
    if (isNotMLSL) {
      p4AugLag = true;
    } else {
      p4MLSL = true;
    }
  }

  if (p4AugLag && p4HasConstraints) {
    // search for aumented lagrangian algorithms
    for (size_t i = 0; i < availableAlgs.size(); i++) {
      const NLOptWrAlgorithm &alg = availableAlgs.at(i);

      bool algAlgAugLag = alg.isAugLag();
      bool algHasConstraints = alg.canHandleNonEqualConstraints();
      bool isRealAugLag = (alg.getAlgorithmEnum() == nlopt::AUGLAG);
      SSTRAT algSearchStrategy = alg.getSearchStrategy();
      bool algSubOptFlag = alg.getNeedsSubopt();

      if (algAlgAugLag && algHasConstraints &&
          ((p4SearchStrategy == algSearchStrategy) || isRealAugLag) &&
          ((p4UseGradient == alg.getUseGradient()) || isRealAugLag) &&
          (true == algSubOptFlag) && (xDim >= alg.getMinParameters()) &&
          ((alg.getMaxParameters() == 0) || (xDim <= alg.getMaxParameters()))) {
        /*
        cout << "DEBUG: AUGLAG:
        NlOptParamFactory::getAvailableAlgorithms(p4pAlg=" << p4pAlg.toString()
        << ",  availableAlgs, xDim=" << xDim << ", ignoreConstraints=" <<
        ignoreConstraints
        << ", searchLocalOrGlobal=" << searchLocalOrGlobal << ", isNotMLSL=" <<
        isNotMLSL << ") const " << endl;
        */
        results.push_back(alg);
      }
    }
  } else if (p4MLSL) {
    // search for MLSL algorithms
    for (size_t i = 0; i < availableAlgs.size(); i++) {
      const NLOptWrAlgorithm &alg = availableAlgs.at(i);

      bool algMLSL = alg.isMLSL();
      bool isRealMLSL = ((alg.getAlgorithmEnum() == nlopt::G_MLSL) ||
                         (alg.getAlgorithmEnum() == nlopt::G_MLSL_LDS));
      SSTRAT algSearchStrategy = alg.getSearchStrategy();
      bool algSubOptFlag = alg.getNeedsSubopt();

      if (algMLSL && ((p4SearchStrategy == algSearchStrategy) || isRealMLSL) &&
          ((p4UseGradient == alg.getUseGradient()) || isRealMLSL) &&
          (true == algSubOptFlag) && (xDim >= alg.getMinParameters()) &&
          ((alg.getMaxParameters() == 0) || (xDim <= alg.getMaxParameters()))) {
        /*
        cout << "###MLSL: NlOptParamFactory::getAvailableAlgorithms(p4pAlg=" <<
        p4pAlg.toString() << ",  availableAlgs, xDim=" << xDim << ",
        ignoreConstraints=" << ignoreConstraints
        << ", searchLocalOrGlobal=" << searchLocalOrGlobal << ", isNotMLSL=" <<
        isNotMLSL << ") const " << endl;
        */
        results.push_back(alg);
      }
    }

  } else {
    // 2nd: search first match
    for (size_t i = 0; i < availableAlgs.size(); i++) {
      const NLOptWrAlgorithm &alg = availableAlgs.at(i);
      SSTRAT algSearchStrategy = alg.getSearchStrategy();
      // bool algSubOpt=isMetaSearchStrategy(algSearchStrategy);
      bool algSubOptFlag = alg.getNeedsSubopt();

      // TODO
      algStrategySearchLocalOrGlobal =
          isP4pAlgLocalOrGlobal(algSearchStrategy, searchLocalOrGlobal);

      // TODO: Wenn vorher Metastrategie war, dann sollten hier die Parameter
      // getestet werden!!!!

      if ((p4SearchStrategy == algSearchStrategy) ||
          (p4LocalOrGlobalSearch && algStrategySearchLocalOrGlobal)) {
        if ((ignoreConstraints ||
             (p4HasConstraints == alg.canHandleNonEqualConstraints())) &&
            (p4UseGradient == alg.getUseGradient()) &&
            (false == algSubOptFlag)) {
          // TODO: change p4pAlg

          if ((xDim >= alg.getMinParameters()) &&
              ((alg.getMaxParameters() == 0) ||
               (xDim <= alg.getMaxParameters()))) {
            if (d > 20)
              cout << "DEBUG: NlOptParamFactory::getAvailableAlgorithms(p4pAlg="
                   << p4pAlg.toString() << ",  availableAlgs, xDim=" << xDim
                   << ", ignoreConstraints=" << ignoreConstraints
                   << ", searchLocalOrGlobal=" << searchLocalOrGlobal
                   << ", isNotMLSL=" << isNotMLSL
                   << ") const => name=" << alg.getName() << endl;

            results.push_back(alg);
          }
        }
      }
    }
  }

  return results;
}

// --------------------------------------------------------------------------------------------------

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

// ==================================================================================================
// static
const std::vector<class NLOptWrAlgorithm> NLOptWrParamFactory::nlOptAlgorithms{

    // enum(algorithm)                name(algorithm)                STRAT  GRAD
    // NEC    nSUB  Min Max
    {nlopt::LN_AUGLAG_EQ, "LN_AUGLAG_EQ", SSTRAT::N, false, true, true, 1, 0},
    {nlopt::LD_AUGLAG_EQ, "LD_AUGLAG_EQ", SSTRAT::N, true, true, true, 1, 0},
    {nlopt::AUGLAG_EQ, "AUGLAG_EQ", SSTRAT::N, true, true, true, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LD_AUGLAG, "LD_AUGLAG", SSTRAT::LM, true, true, true, 1, 0},
    {nlopt::LN_AUGLAG, "LN_AUGLAG", SSTRAT::LM, false, true, true, 1, 0},
    {nlopt::AUGLAG, "AUGLAG", SSTRAT::GM, true, true, true, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GD_MLSL, "GD_MLSL", SSTRAT::GM, true, false, true, 1, 0},
    {nlopt::GN_MLSL, "GN_MLSL", SSTRAT::GM, false, false, true, 1, 0},
    {nlopt::GD_MLSL_LDS, "GD_MLSL_LDS", SSTRAT::GM, true, false, true, 1, 0},
    {nlopt::GN_MLSL_LDS, "GN_MLSL_LDS", SSTRAT::GM, false, false, true, 1, 0},
    {nlopt::G_MLSL, "G_MLSL", SSTRAT::GM, true, false, true, 1, 0},
    {nlopt::G_MLSL_LDS, "G_MLSL_LDS", SSTRAT::GM, true, false, true, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LD_LBFGS, "LD_LBFGS", SSTRAT::L, true, false, false, 1, 0},
    {nlopt::LD_LBFGS_NOCEDAL, "LD_LBFGS_NOCEDAL", SSTRAT::L, true, false, false,
     1, 0},
    {nlopt::LD_TNEWTON, "LD_TNEWTON", SSTRAT::L, true, false, false, 1, 0},
    {nlopt::LD_TNEWTON_PRECOND, "LD_TNEWTON_PRECOND", SSTRAT::L, true, false,
     false, 1, 0},
    {nlopt::LD_TNEWTON_PRECOND_RESTART, "LD_TNEWTON_PRECOND_RESTART", SSTRAT::L,
     true, false, false, 1, 0},
    {nlopt::LD_TNEWTON_RESTART, "LD_TNEWTON_RESTART", SSTRAT::L, true, false,
     false, 1, 0},
    {nlopt::LD_VAR1, "LD_VAR1", SSTRAT::L, true, false, false, 1, 0},
    {nlopt::LD_VAR2, "LD_VAR2", SSTRAT::L, true, false, false, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LD_MMA, "LD_MMA", SSTRAT::L, true, true, false, 1, 0},
    {nlopt::LD_CCSAQ, "LD_CCSAQ", SSTRAT::L, true, true, false, 1, 0},
    {nlopt::LD_SLSQP, "LD_SLSQP", SSTRAT::L, true, true, false, 1, 900},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LN_COBYLA, "LN_COBYLA", SSTRAT::L, false, true, false, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::LN_SBPLX, "LN_SBPLX", SSTRAT::L, false, false, false, 1, 15},
    {nlopt::LN_BOBYQA, "LN_BOBYQA", SSTRAT::L, false, false, false, 1, 12},
    {nlopt::LN_NELDERMEAD, "LN_NELDERMEAD", SSTRAT::L, false, false, false, 1,
     12},
    {nlopt::LN_NEWUOA, "LN_NEWUOA", SSTRAT::L, false, false, false, 1, 12},
    {nlopt::LN_NEWUOA_BOUND, "LN_NEWUOA_BOUND", SSTRAT::L, false, false, false,
     1, 12},
    {nlopt::LN_PRAXIS, "LN_PRAXIS", SSTRAT::L, false, false, false, 1, 12},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GN_DIRECT, "GN_DIRECT", SSTRAT::G, false, false, false, 1, 0},
    {nlopt::GN_DIRECT_L, "GN_DIRECT_L", SSTRAT::G, false, false, false, 1, 0},
    {nlopt::GN_DIRECT_L_NOSCAL, "GN_DIRECT_L_NOSCAL", SSTRAT::G, false, false,
     false, 1, 0},
    {nlopt::GN_DIRECT_L_RAND, "GN_DIRECT_L_RAND", SSTRAT::G, false, false,
     false, 1, 0},
    {nlopt::GN_DIRECT_L_RAND_NOSCAL, "GN_DIRECT_L_RAND_NOSCAL", SSTRAT::G,
     false, false, false, 1, 0},
    {nlopt::GN_DIRECT_NOSCAL, "GN_DIRECT_NOSCAL", SSTRAT::G, false, false,
     false, 1, 0},
    {nlopt::GN_ORIG_DIRECT, "GN_ORIG_DIRECT", SSTRAT::G, false, false, false, 1,
     0},
    {nlopt::GN_ORIG_DIRECT_L, "GN_ORIG_DIRECT_L", SSTRAT::G, false, false,
     false, 1, 0},
    {nlopt::GN_AGS, "GN_AGS", SSTRAT::G, false, false, false, 1, 5},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GD_STOGO, "GD_STOGO", SSTRAT::G, true, false, false, 1, 20},
    {nlopt::GD_STOGO_RAND, "GD_STOGO_RAND", SSTRAT::G, true, false, false, 1,
     20},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GN_CRS2_LM, "GN_CRS2_LM", SSTRAT::R, false, false, false, 1, 0},
    {nlopt::GN_ESCH, "GN_ESCH", SSTRAT::R, false, false, false, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

    {nlopt::GN_ISRES, "GN_ISRES", SSTRAT::R, false, true, false, 1, 0},

    // ----------------------------------------------------------------------------------------------------------

};

} // namespace nloptwr
