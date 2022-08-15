
#include "nloptwrtest/griewank.h"
#include "nloptwr/nloptwr.h"

#include "nloptwr/optfktnbase.h"
#include "nloptwr/optfktnclass.h"

#include <cmath>
#include <vector>

#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

namespace opttest {

Griewank::Griewank(int dim) : oif::OptFknClass(), nDim(dim) {}

// virtual
void Griewank::initialize(double lb, double ub, double xInit) {

  // call function of base class
  init(nDim, 0, lb, ub, xInit);
}

// virtual
Griewank::~Griewank() {}

// virtual
oif::OptFknBase *Griewank::clone() const { return (new Griewank(*this)); }

// virtual
double Griewank::optFktn(const std::vector<double> &x, std::vector<double> &c) {
  double y = 0.0;
  double sum = 0.0;
  double prod = 1.0;
  double xi = 1.0;

  for (size_t i = 0; i < nDim; i++) {
    xi = x[i];
    sum = sum + xi * xi / 4000.;
    prod = prod * cos(xi / sqrt(1.0 + i));
  }

  y = sum - prod + 1.;

  // cout << "## y=" << y << endl;

  return y;
}

} // namespace opttest
