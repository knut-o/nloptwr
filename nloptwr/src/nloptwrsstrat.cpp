
#include "nloptwr/nloptwrsstrat.h"

#include <string>
#include <iostream>     // std::cout
#include <iomanip>
#include <sstream>      // std::stringstream

using namespace std;

namespace nloptwr
{

NLOptWrSStrat::NLOptWrSStrat ( SSTRAT searchStrat, bool useGrad, bool useAugLagBeforeMlsl )
  :
  searchStrategy ( searchStrat ),
  useGradient ( useGrad ),
  useAugLagBeforeMLSL ( useAugLagBeforeMlsl )
{
  if (SSTRAT::R == searchStrat )
    {
      useGradient=false;
    }

}

NLOptWrSStrat::NLOptWrSStrat ( const NLOptWrSStrat& src )
{
  searchStrategy=src.searchStrategy;
  useGradient=src.useGradient;
  useAugLagBeforeMLSL=src.useAugLagBeforeMLSL;
};

// virtual
NLOptWrSStrat::~NLOptWrSStrat() { }

SSTRAT NLOptWrSStrat::getSearchStrategy() const
{
  return searchStrategy;
}

void NLOptWrSStrat::setSearchStrategy ( SSTRAT val )
{
  searchStrategy=val;
}

bool NLOptWrSStrat::getUseAugLagBeforeMLSL()  const
{
  return useAugLagBeforeMLSL;
}

bool NLOptWrSStrat::getUseGradient()  const
{
  return useGradient;
}


std::string NLOptWrSStrat::toString() const
{
  stringstream ss;

  ss
      << "NloptWrSStrat( "
      << "searchStrategy=" << left << setw ( 2 ) << getStrategyAsString ( searchStrategy )
      << ", useGradient=" << useGradient
      << ", useAugLagBeforeMLSL=" << useAugLagBeforeMLSL
      << " ) ";

  return ss.str();
}


}

