
#include "nloptwrtest/oproblem.h"

#include "nloptwr/optfktnbase.h"
#include "nloptwr/optfktnclass.h"

#include <cmath>
#include <vector>

#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

namespace opttest {

OProblem::OProblem(int dim) : oif::OptFknClass(), nDim(dim) {
  initialize();
  validate();
}

// virtual
void OProblem::initialize() {
  init(nDim, 0, 2, 0.0, 5.0, 1.7);

  // initialize special bounds
  setLb(0, -0.5);
  setXInit(0, 0.4234);
  setUb(0, 2.0);

  // initialize special bounds
  setLb(1, 0.19);
  setXInit(1, 1.78);
  setUb(1, 6.0);
}

// virtual
OProblem::~OProblem() {}

// The clone function is essential for parallelization
oif::OptFknBase *OProblem::clone() const { return (new OProblem(*this)); }

// virtual
void OProblem::optFktn(const std::vector<double> &x, std::vector<double> &fc) {
  for (size_t i = 0; i < getSizeOfNeConstraints(); i++) {
    double a = constraint_data[i][0];
    double b = constraint_data[i][1];
    fc[i + 1] = (pow(a * x[0] + b, 3) - x[1]);
  };
  double val = sqrt(x[1]);

  // double xTmp=0.0;
  double tmp1 = 0.0;
  double tmp2 = 0.0;
  double tmpX1 = 1.0;
  double tmpX2 = 1.0;

  if (getSizeOfX() == 3) {
    tmp2 = (1.0 - x[2]);
    val += 100.0 * (tmp2 * tmp2);

  } else {
    tmpX1 = x[2];
    for (size_t i = 3; i < getSizeOfX(); i++) {
      tmpX2 = x[i];
      tmp1 = (tmpX2 - tmpX1 * tmpX1);
      tmp2 = (1.0 - tmpX1);
      val += 100.0 * (tmp1 * tmp1) + (tmp2 * tmp2);
      tmpX2 = tmpX1;
    }
  }

  // cout << "#### OProblem::optFktn val= " << setw(7) << val << endl;

  fc[0] = val;
}

} // namespace opttest
