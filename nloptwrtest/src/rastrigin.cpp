
#include "nloptwrtest/rastrigin.h"
#include "nloptwr/nloptwr.h"

#include "nloptwr/optfktnbase.h"
#include "nloptwr/optfktnclass.h"

#include <cmath>
#include <vector>

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

namespace opttest
{
    
    Rastrigin::Rastrigin ( int dim )
  :
  oif::OptFknClass(),
  nDim ( dim )
{

}

// virtual
void Rastrigin::initialize ( double lb, double ub, double xInit )
{

  // call function of base class
  init ( nDim, 0, lb, ub, xInit );

}

// virtual
Rastrigin::~Rastrigin() {}

// virtual
oif::OptFknBase* Rastrigin::clone() const
{
  return ( new Rastrigin ( *this ) );
}

// virtual
double Rastrigin::optFktn ( const std::vector<double>& x, std::vector<double>& c )
{
    size_t n=x.size();
    double xi=0.0;
    double sum=10.0*n;
    for ( size_t i=0; i<n; i++)  {
        xi = x[i];
        sum += (xi*xi - 10*cos(M_2_PI*xi));
    }
        
    return sum;
}


} // namespace opttest

