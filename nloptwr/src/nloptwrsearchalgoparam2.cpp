
#include "nloptwr/nloptwrsearchalgoparam2.h"
#include "nloptwr/nloptwrsearchalgoparam.h"

#include <string>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <iomanip>

using namespace std;

namespace nloptwr
{

NLOptWrSearchAlgoParam2::NLOptWrSearchAlgoParam2 ( SSTRAT searchStrat, bool hasConstr, bool useGrad, bool useAugLagBeforeMlsl )
  :
  NLOptWrSearchAlgoParam ( searchStrat, hasConstr, useGrad ),
  useAugLagBeforeMLSL ( useAugLagBeforeMlsl )
{
 // cout << "DEBUG: NLOptWrSearchAlgoParam2::NLOptWrSearchAlgoParam2(searchStrat=" << searchStrat << ", hasConstr=" << hasConstr << ", useGrad=" << useGrad << ", useAugLagBeforeMlsl=" << useAugLagBeforeMlsl << ") : toString()=" << toString() << endl;
}

NLOptWrSearchAlgoParam2::NLOptWrSearchAlgoParam2 ( const NLOptWrSearchAlgoParam& src, bool useAugLagBeforeMlsl )
{
  searchStrategy=src.getSearchStrategy();
  useGradient=src.getUseGradient();
  hasContraints=src.getHasContraints();
  useAugLagBeforeMLSL=useAugLagBeforeMlsl;

  // cout << "### DEBUG: NLOptWrSearchAlgoParam2::NLOptWrSearchAlgoParam2(const NLOptWrSearchAlgoParam& src, bool useAugLagBeforeMlsl) : toString()=" << toString() << endl;
}

NLOptWrSearchAlgoParam2::NLOptWrSearchAlgoParam2 ( const NLOptWrSearchAlgoParam2& src )
{
  searchStrategy=src.getSearchStrategy();
  useGradient=src.getUseGradient();
  hasContraints=src.getHasContraints();
  useAugLagBeforeMLSL=src.useAugLagBeforeMLSL;

  // cout << "### DEBUG: NLOptWrSearchAlgoParam2::NLOptWrSearchAlgoParam2(const NLOptWrSearchAlgoParam2& src) : toString()=" << toString() << endl;
};

NLOptWrSearchAlgoParam2::~NLOptWrSearchAlgoParam2() { }

bool NLOptWrSearchAlgoParam2::getUseAugLagBeforeMLSL() const
{
  return useAugLagBeforeMLSL;
}

bool NLOptWrSearchAlgoParam2::operator < ( const NLOptWrSearchAlgoParam2& rhs ) const
{
  int  r=0;

  const NLOptWrSearchAlgoParam& lhs= *this;
  const NLOptWrSearchAlgoParam& rhs2= rhs;

  if ( lhs < rhs2 )
    {
      r=1;
    }
  else
    {
      r= ( rhs2 < lhs ) ? -1 : 0;
    }
  if ( r==0 )  if ( useAugLagBeforeMLSL!=rhs.useAugLagBeforeMLSL )
      {
        r= ( useAugLagBeforeMLSL ) ? 1 : -1;
      }


  bool result = ( r<0 );

  return result;
}


std::string NLOptWrSearchAlgoParam2::toString() const
{
  stringstream ss;

  ss
      << "P4PrefAlgorithm2( "
      << "searchStrategy=" << left << setw ( 2 ) << getStrategyAsString ( getSearchStrategy() )
      << ", hasContraints=" << static_cast<int>(getHasContraints())
      << ", useGrad=" << static_cast<int>(getUseGradient())
      << ", useAugLagBeforeMLSL=" << static_cast<int>(getUseAugLagBeforeMLSL())
      << " ) ";

  return ss.str();
}


}

