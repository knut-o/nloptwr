

#include "nloptwrtest/rastrigin.h"

#include "nloptwr/optfktnbase.h"
#include "nloptwr/optfktnclass.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

namespace opttest {
// static
const double Rastrigin::Pi2 =
#ifdef _MSC_VER
    2.0 * 3.14159265358979323846;
#else
    2.0 * M_PI;
#endif // _MSC_VER

Rastrigin::Rastrigin(int dim) : oif::OptFknClass(), nDim(dim) {}

// virtual
void Rastrigin::initialize(double lb, double ub, double xInit) {

  // call function of base class
  init(nDim, 0, lb, ub, xInit);
}

// virtual
Rastrigin::~Rastrigin() {}

// virtual
oif::OptFknBase *Rastrigin::clone() const { return (new Rastrigin(*this)); }

// virtual
double Rastrigin::optFktn(const std::vector<double> &x,
                          std::vector<double> &c) {
  size_t n = x.size();
  double sum = 0.0;

  for (size_t i = 0; i < n; i++) {
    sum += (x[i] * x[i] + 10 * (1.0 - cos(Pi2 * x[i])));
  }

  return sum;
}

} // namespace opttest
