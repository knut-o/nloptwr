
#include "nloptwr/nloptwralgorithm.h"   
#include "nlopt.hpp"

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>


using namespace std;

namespace nloptwr {
    
    SEARCH_STRATEGY getMetaSearchStrategy(SEARCH_STRATEGY strat) {
        if (static_cast<int>(strat) > 0) {
            if (strat == R) strat=N; 
            if (strat == L) strat=LM; 
            if (strat == G) strat=GM; 
        }
        return strat;  
    }
    
    SEARCH_STRATEGY getNonMetaSearchStrategy(SEARCH_STRATEGY strat) {
        if (static_cast<int>(strat) < 0) {
            if (strat == LM) strat=L; 
            if (strat == GM) strat=G; 
        }
        return strat;  
    }
    
    bool isSearchStrategyLGM(SEARCH_STRATEGY strat) {
        return ((strat == GM)||(strat == LM)||(strat == L)||(strat == G)); 
    }

    bool isSearchStrategyLG(SEARCH_STRATEGY strat) {
        return ((strat == G)||(strat == L)); 
    }

    bool isMetaSearchStrategy(SEARCH_STRATEGY strat) {
        return ((strat == GM)||(strat == LM)); 
    }

    bool isSearchStrategyR(SEARCH_STRATEGY strat) {
        return (strat == R); 
    }
    
    bool  isP4pAlgLocalOrGlobal(SEARCH_STRATEGY strat, bool searchLocalOrGlobal) {
        int  searchStrategyIntAbs=abs(static_cast<int>(strat));
        return searchLocalOrGlobal&&(
                searchStrategyIntAbs>=static_cast<int>(nloptwr::L) 
                || 
                searchStrategyIntAbs<=static_cast<int>(nloptwr::G)
                );
    }

    string getStrategyAsString(SEARCH_STRATEGY strat) {
        string rc=" ";
        if (strat==R) { rc="R"; } else 
        if (strat==G) { rc="G"; } else 
        if (strat==L) { rc="L"; } else 
        if (strat==N) { rc="N"; } else 
        if (strat==LM) { rc="LM"; } else 
        if (strat==GM) { rc="GM"; } else 
        rc="?"; 
            
        return rc;
    }


    NLOptWrAlgorithm::NLOptWrAlgorithm(
    nlopt::algorithm a,
    const std::string& n,
    SEARCH_STRATEGY rgl,
    bool hasGrad,
    bool neC,
    bool nSubopt,
    unsigned int minP,
    unsigned int maxP
    ) 
    :
    alg(a),
    name(n),
    pRGL(rgl),
    hasGradient(hasGrad),
    neConstraints(neC),
    needsSubOpt(nSubopt),
    minParam(minP),
    maxParam(maxP)
    { 
        
    }
    
    NLOptWrAlgorithm::NLOptWrAlgorithm() { }
    
    NLOptWrAlgorithm::NLOptWrAlgorithm(const NLOptWrAlgorithm& src) 
    :
   alg(src.alg),
   name(src.name),
   pRGL(src.pRGL),
   hasGradient(src.hasGradient),
   neConstraints(src.neConstraints),
   needsSubOpt(src.needsSubOpt),
   minParam(src.minParam),
   maxParam(src.maxParam)    
   {
        
    }

    
    NLOptWrAlgorithm::~NLOptWrAlgorithm() { 
        
    } 
    
    nlopt::algorithm NLOptWrAlgorithm::getAlgorithmEnum() const {  return alg; }
    
    const std::string& NLOptWrAlgorithm::getName() const {  return name; }
    
    SEARCH_STRATEGY NLOptWrAlgorithm::getSearchStrategy() const {  return pRGL; }
    
    bool NLOptWrAlgorithm::getUseGradient() const { return hasGradient; }
    
    bool NLOptWrAlgorithm::canHandleNonEqualConstraints() const {  return neConstraints; }
    
    bool NLOptWrAlgorithm::getNeedsSubopt() const {  return needsSubOpt; }
    
    unsigned int NLOptWrAlgorithm::getMinParameters() const {  return minParam; }
    
    void NLOptWrAlgorithm::setMinParameters(unsigned int var) { minParam=var; }

    
    unsigned int NLOptWrAlgorithm::getMaxParameters() const {  return maxParam; }
    
    void NLOptWrAlgorithm::setMaxParameters(unsigned int var) { maxParam=var; }

    
    void NLOptWrAlgorithm::setHasGradient(bool val) { hasGradient=val; }
    
    void NLOptWrAlgorithm::setNeConstraints(bool val) { neConstraints=val; }

    bool NLOptWrAlgorithm::isAugLag() const {
        return (name.find("AUGLAG") != string::npos);
    }


    bool NLOptWrAlgorithm::isMLSL() const {
        return (name.find("MLSL") != string::npos);
    }

    // ==================================================================================================
    // ==================================================================================================
    
    bool NLOptWrAlgorithm::isSetPopulation() const {
        return
                (name.find("CRS2" ) != string::npos) || 
                (name.find("MSL"  ) != string::npos) ||  
                (name.find("ISRES") != string::npos);
    }
  
  bool NLOptWrAlgorithm::isHasInnerMaxeval() const {
        return
                (name.find("MMA" )  != string::npos) || 
                (name.find("CCSAQ") != string::npos);
  }

  bool NLOptWrAlgorithm::isVectorStorage() const {
        return  (
                (name.find("LBFGS")  != string::npos) ||
                (name.find("MMA")    != string::npos) ||
                (name.find("TNEWTON")!= string::npos) ||
                (name.find("VAR1")   != string::npos) ||
                (name.find("VAR2")   != string::npos) ||
                (name.find("SLSQP")  != string::npos) ||
                (name.find("CCSAQ")  != string::npos)
                );
  }

    bool NLOptWrAlgorithm::operator < (const NLOptWrAlgorithm& rhs) const {
        return static_cast<int>(alg) <  static_cast<int>(rhs.alg);
    }

    bool NLOptWrAlgorithm::operator == (const NLOptWrAlgorithm& rhs) const {
        return (alg == rhs.alg);
    }
       
    std::string NLOptWrAlgorithm::toString() const {
        stringstream ss;
        ss 
        << "NlOptAlgorithm("
        // << "  algorithm=" << setw(3) << static_cast<int>(alg) << ", "
        // << "/* , name= */
        << "nlopt::"<< setw(12) << left << name << ", "
        // << "/* , SEARCH_STRATEGY= */" 
        << setw(2) << getStrategyAsString(pRGL) << ", "
        // << "/* hasGradient= */" 
        << setw(5) << (hasGradient? "true" : "false") << ", "
        // << "/* , neConstraints= */" 
        << setw(5) << (neConstraints? "true" : "false") << ", "
        // << "/* , needsSubOpt= */" 
        << setw(5) << (needsSubOpt? "true" : "false") << ", "
        // << "/* , minParam= */" 
        << setw(3) << minParam << ", "
        // << "/* , maxParam= */" 
        << setw(5) << maxParam
        << " ) ";

        return ss.str();
    }
} // namespace
