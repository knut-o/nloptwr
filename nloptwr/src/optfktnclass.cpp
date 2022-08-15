
#include "nloptwr/optfktnclass.h"
#include "nloptwr/optfktnbase.h"

#include <memory>
#include <vector>

#include <iomanip>
#include <iostream>

using namespace std;

namespace oif {

OptFknClass::OptFknClass() : oif::OptFknBase() {
  // init(1, 0, -1000.0, 1000.0, 0.1);
}

void OptFknClass::init(size_t dim, size_t nrOfNeConstraints, double lb,
                       double ub, double xInit) {
  nDim = dim;
  mDim = nrOfNeConstraints;
  lB = shared_ptr<vector<double>>(new vector<double>(dim, lb));
  uB = shared_ptr<vector<double>>(new vector<double>(dim, ub));
  xInitial = shared_ptr<vector<double>>(new vector<double>(dim, xInit));
  //
  if (c.size() != mDim)
    c.resize(mDim);
}

// virtual
bool OptFknClass::validate() const {
  bool rc = true; // (lB->size()==nDim)&&(uB->size()==nDim);

  size_t n = (*xInitial).size();
  if (nDim != n) {
    cerr << "ERROR: (n=" << setw(4) << n << ") != (nDim=" << nDim << ") "
         << endl;
    rc = false;
  }

  double lVal = 0.0; // lower bound
  double xVal = 0.0; // initial value
  double uVal = 0.0; // upper bound
  for (size_t i = 0; i < nDim; i++) {
    lVal = (*lB)[i];
    xVal = (*xInitial)[i];
    uVal = (*uB)[i];
    if (rc) {
      rc = ((lVal < xVal) && (xVal < uVal));
      if (!rc) {
        cout << "#### rc=" << rc << ", index i=" << setw(4) << i
             << ": false = ((lVal=" << lVal << ") <= (xVal=" << xVal
             << ") <=  (uVal=" << uVal << "))" << endl;
        cerr.flush();
      }
    }
  }

  return rc;
}

// virtual
OptFknClass::~OptFknClass() {}

void OptFknClass::setXInit(std::size_t idx, double val) {
  (*xInitial)[idx] = val;
}

/**
 * set a lower bound at idx
 */
void OptFknClass::setLb(std::size_t idx, double val) { (*lB)[idx] = val; }

/**
 * set a upper bound at idx
 */
void OptFknClass::setUb(std::size_t idx, double val) { (*uB)[idx] = val; }

// virtual
size_t OptFknClass::getSizeOfX() const { return nDim; }

// virtual
size_t OptFknClass::getSizeOfNeConstraints() const { return mDim; }

// virtual
vector<double> OptFknClass::getUpperBounds() const { return *uB; }

// virtual
std::vector<double> OptFknClass::getLowerBounds() const { return *lB; }

// virtual
std::vector<double> OptFknClass::getXInitial() const { return *xInitial; }

// ===========================================================

// virtual
double OptFknClass::optF(const std::vector<double> &x) { return optFktn(x, c); }

// virtual
void OptFknClass::optC(const std::vector<double> &x, std::vector<double> &c) {
  if (!c.empty())
    optFktn(x, c);
}
} // namespace oif
