
#include "nloptwr/nloptwralgorithm.h"
#include "nlopt.hpp"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace nloptwr {

SSTRAT getMetaSearchStrategy(SSTRAT strat) {
  if (static_cast<int>(strat) > 0) {
    if (strat == SSTRAT::R)
      strat = SSTRAT::N;
    if (strat == SSTRAT::L)
      strat = SSTRAT::LM;
    if (strat == SSTRAT::G)
      strat = SSTRAT::GM;
  }
  return strat;
}

SSTRAT getNonMetaSearchStrategy(SSTRAT strat) {
  if (static_cast<int>(strat) < 0) {
    if (strat == SSTRAT::LM)
      strat = SSTRAT::L;
    if (strat == SSTRAT::GM)
      strat = SSTRAT::G;
  }
  return strat;
}

bool isSearchStrategyLGM(SSTRAT strat) {
  return ((strat == SSTRAT::GM) || (strat == SSTRAT::LM) ||
          (strat == SSTRAT::L) || (strat == SSTRAT::G));
}

bool isSearchStrategyLG(SSTRAT strat) {
  return ((strat == SSTRAT::G) || (strat == SSTRAT::L));
}

bool isMetaSearchStrategy(SSTRAT strat) {
  return ((strat == SSTRAT::GM) || (strat == SSTRAT::LM));
}

bool isSearchStrategyR(SSTRAT strat) { return (strat == SSTRAT::R); }

bool isP4pAlgLocalOrGlobal(SSTRAT strat, bool searchLocalOrGlobal) {
  int searchStrategyIntAbs = abs(static_cast<int>(strat));
  return searchLocalOrGlobal &&
         (searchStrategyIntAbs >= static_cast<int>(SSTRAT::L) ||
          searchStrategyIntAbs <= static_cast<int>(SSTRAT::G));
}

string getStrategyAsString(SSTRAT strat) {
  string rc = " ";
  if (strat == SSTRAT::R) {
    rc = "R";
  } else if (strat == SSTRAT::G) {
    rc = "G";
  } else if (strat == SSTRAT::L) {
    rc = "L";
  } else if (strat == SSTRAT::N) {
    rc = "N";
  } else if (strat == SSTRAT::LM) {
    rc = "LM";
  } else if (strat == SSTRAT::GM) {
    rc = "GM";
  } else
    rc = "?";

  return rc;
}

SSTRAT getStrategyFromString(const std::string &src, SSTRAT sstratDefault) {
  SSTRAT result = sstratDefault;
  if ((1 <= src.size()) && (src.size() <= 2)) {
    string srcU(src);
    if (src.size() >= 1)
      srcU[0] = toupper(srcU[0]);
    if (src.size() >= 2)
      srcU[1] = toupper(srcU[1]);

    if (srcU == "R") {
      result = SSTRAT::R;
    } else if (srcU == "G") {
      result = SSTRAT::G;
    } else if (srcU == "L") {
      result = SSTRAT::L;
    } else if (srcU == "N") {
      result = SSTRAT::N;
    } else if (srcU == "LM") {
      result = SSTRAT::LM;
    } else if (srcU == "GM") {
      result = SSTRAT::GM;
    };
  }
  return result;
}

NLOptWrAlgorithm::NLOptWrAlgorithm(nlopt::algorithm a, const std::string &n,
                                   SSTRAT rgl, bool hasGrad, 
                                   bool neC, 
                                   // bool necO,
                                   bool nSubopt, unsigned int minP,
                                   unsigned int maxP)
    : alg(a), name(n), pRGL(rgl), hasGradient(hasGrad), 
      neConstraints(neC),
      // neOnlyConstraints(necO),
      needsSubOpt(nSubopt), minParam(minP), maxParam(maxP) {}

// NLOptWrAlgorithm::NLOptWrAlgorithm() { }

/*
NLOptWrAlgorithm::NLOptWrAlgorithm(const NLOptWrAlgorithm &src)
    : alg(src.alg), name(src.name), pRGL(src.pRGL),
      hasGradient(src.hasGradient), neConstraints(src.neConstraints),
      needsSubOpt(src.needsSubOpt), minParam(src.minParam),
      maxParam(src.maxParam) {}
*/

NLOptWrAlgorithm::~NLOptWrAlgorithm() {}

nlopt::algorithm NLOptWrAlgorithm::getAlgorithmEnum() const { return alg; }

const std::string &NLOptWrAlgorithm::getName() const { return name; }

SSTRAT NLOptWrAlgorithm::getSearchStrategy() const { return pRGL; }

bool NLOptWrAlgorithm::getUseGradient() const { return hasGradient; }

bool NLOptWrAlgorithm::canHandleNonEqualConstraints() const {
  return neConstraints;
}

bool NLOptWrAlgorithm::getNeedsSubopt() const { return needsSubOpt; }

unsigned int NLOptWrAlgorithm::getMinParameters() const { return minParam; }

void NLOptWrAlgorithm::setMinParameters(unsigned int var) { minParam = var; }

unsigned int NLOptWrAlgorithm::getMaxParameters() const { return maxParam; }

void NLOptWrAlgorithm::setMaxParameters(unsigned int var) { maxParam = var; }

void NLOptWrAlgorithm::setHasGradient(bool val) { hasGradient = val; }

void NLOptWrAlgorithm::setNeConstraints(bool val) { neConstraints = val; }

bool NLOptWrAlgorithm::isAugLag() const {
  return (name.find("AUGLAG") != string::npos);
}

bool NLOptWrAlgorithm::isMLSL() const {
  return (name.find("MLSL") != string::npos);
}

// ==================================================================================================
// ==================================================================================================

bool NLOptWrAlgorithm::isSetPopulation() const {
  return (name.find("CRS2") != string::npos) ||
         (name.find("MSL") != string::npos) ||
         (name.find("ISRES") != string::npos);
}

bool NLOptWrAlgorithm::isHasInnerMaxeval() const {
  return (name.find("MMA") != string::npos) ||
         (name.find("CCSAQ") != string::npos);
}

bool NLOptWrAlgorithm::isVectorStorage() const {
  return ((name.find("LBFGS") != string::npos) ||
          (name.find("MMA") != string::npos) ||
          (name.find("TNEWTON") != string::npos) ||
          (name.find("VAR1") != string::npos) ||
          (name.find("VAR2") != string::npos) ||
          (name.find("SLSQP") != string::npos) ||
          (name.find("CCSAQ") != string::npos));
}

bool NLOptWrAlgorithm::operator<(const NLOptWrAlgorithm &rhs) const {
  return static_cast<int>(alg) < static_cast<int>(rhs.alg);
}

bool NLOptWrAlgorithm::operator==(const NLOptWrAlgorithm &rhs) const {
  return (alg == rhs.alg);
}

std::string NLOptWrAlgorithm::toString() const {
  stringstream ss;
  ss << "NlOptAlgorithm("
     // << "  algorithm=" << setw(3) << static_cast<int>(alg) << ", "
     // << "/* , name= */
     << "nlopt::" << setw(12) << left << name
     << ", "
     // << "/* , SEARCH_STRATEGY= */"
     << setw(2) << getStrategyAsString(pRGL)
     << ", "
     // << "/* hasGradient= */"
     << setw(5) << (hasGradient ? "true" : "false")
     << ", "
     // << "/* , neConstraints= */"
     << setw(5) << (neConstraints ? "true" : "false")
     << ", "
     // << "/* , needsSubOpt= */"
     << setw(5) << (needsSubOpt ? "true" : "false")
     << ", "
     // << "/* , minParam= */"
     << setw(3) << minParam
     << ", "
     // << "/* , maxParam= */"
     << setw(5) << maxParam << " ) ";

  return ss.str();
}
} // namespace nloptwr
