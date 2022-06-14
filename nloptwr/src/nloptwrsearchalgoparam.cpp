
#include "nloptwr/nloptwrsearchalgoparam.h"

#include <string>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <iomanip>

using namespace std;

namespace nloptwr
{

NLOptWrSearchAlgoParam::NLOptWrSearchAlgoParam ( SEARCH_STRATEGY searchStrat, bool hasConstr, bool useGrad )
  :
  searchStrategy ( searchStrat ),
  hasContraints ( hasConstr ),
  useGradient ( useGrad )
{
  if ( R == searchStrat )
    {
      useGradient=false;
    }

}

NLOptWrSearchAlgoParam::NLOptWrSearchAlgoParam() { }

NLOptWrSearchAlgoParam::NLOptWrSearchAlgoParam ( const NLOptWrSearchAlgoParam& src )
{
  searchStrategy=src.searchStrategy;
  hasContraints=src.hasContraints;
  useGradient=src.useGradient;
};

NLOptWrSearchAlgoParam::~NLOptWrSearchAlgoParam() { }

bool NLOptWrSearchAlgoParam::operator < ( const NLOptWrSearchAlgoParam& rhs ) const
{
  int  r=0;
  if ( r==0 )  if ( searchStrategy!=rhs.searchStrategy )
      {
        r= ( searchStrategy<rhs.searchStrategy ) ? 1 : -1;
      }
  if ( r==0 )  if ( hasContraints!=rhs.hasContraints )
      {
        r= ( hasContraints ) ? 1 : -1;
      }
  if ( r==0 )  if ( useGradient!=rhs.useGradient )
      {
        r= ( useGradient ) ? 1 : -1;
      }

  bool result = ( r<0 );

  return result;
}

SEARCH_STRATEGY NLOptWrSearchAlgoParam::getSearchStrategy() const
{
  return searchStrategy;
}

void NLOptWrSearchAlgoParam::setSearchStrategy ( SEARCH_STRATEGY val )
{
  searchStrategy=val;
}

bool NLOptWrSearchAlgoParam::getHasContraints()  const
{
  return hasContraints;
}

bool NLOptWrSearchAlgoParam::getUseGradient()  const
{
  return useGradient;
}

void NLOptWrSearchAlgoParam::setUseContraints ( bool val )
{
  hasContraints=val;
}


std::string NLOptWrSearchAlgoParam::toString() const
{
  stringstream ss;

  ss
      << "P4PrefAlgorithm( "
      << "searchStrategy=" << setw ( 2 ) << getStrategyAsString ( searchStrategy )
      << ", hasContraints=" << hasContraints
      << ", useGradient=" << useGradient
      << " ) ";

  return ss.str();
}


std::string NLOptWrSearchAlgoParam::toStringShort() const
{
  stringstream ss;

  ss
      << "P4PrefAlgorithm("
      << "Strat=" << left << setw ( 2 ) << getStrategyAsString ( searchStrategy )
      << ", C=" << hasContraints
      << ", Grad=" << useGradient
      << ") ";

  return ss.str();
}


}

