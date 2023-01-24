
#include "nloptwr/nloptwrfcargs.h"

#include "nloptwr/omphelper.h"
#include "nloptwr/optfktnbase.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ===================================================================
// ===================================================================

namespace nloptwr {

// ===================================================================
// ===================================================================

FArg::FArg(size_t xDim1, size_t fCSize1)
    : fcSize(fCSize1), xTmp1(xDim1), fcTmp1(fCSize1), xTmp2(xDim1),
      fcTmp2(fCSize1) {}

FArg::~FArg() {}

// ===================================================================
// ===================================================================

NLOptWrFCArgs::NLOptWrFCArgs() { toRecalculateGradients = false; }

NLOptWrFCArgs::~NLOptWrFCArgs() {}

void NLOptWrFCArgs::init(vector<shared_ptr<oif::OptFknBase>> &optFknB,
                         const vector<double> &dX) {
  optFknBases = optFknB;
  difX = dX;
  difXR = dX;

  for (size_t i = 0; i < difXR.size(); i++)
    difXR[i] = 0.5 / dX[i];

  xDim = optFknB[0]->getSizeOfX();

  numThreads = optFknBases.size();

  noOfEqConstraints = optFknB[0]->getSizeOfEqConstraints();
  noOfNeConstraints = optFknB[0]->getSizeOfNeConstraints();
  noOfConstraints = (noOfEqConstraints + noOfNeConstraints);

  fcArgSize = (1 + noOfConstraints);

  fArgPositions = {0, 1, 1 + noOfEqConstraints,
                   1 + noOfEqConstraints + noOfNeConstraints};

  // temporary object
  FArg fArg(xDim, fcArgSize);
  fArgs.resize(numThreads, fArg);

  // resize array of function values
  fc.resize(fcArgSize);

  // resize 2-D array of gradients
  fcGradients.resize(xDim, fc);

  if (x.size() != xDim)
    x.resize(xDim, numeric_limits<double>::min());

  toRecalculateGradients = false;
}

// ===================================================================

bool NLOptWrFCArgs::hasEqConstraints() const { return (noOfEqConstraints > 0); }

bool NLOptWrFCArgs::hasNeConstraints() const { return (noOfNeConstraints > 0); }

// ===================================================================

bool NLOptWrFCArgs::checkXVec(unsigned int n, const double *x1) const {
  bool rc = (noOfConstraints > 0);

  for (unsigned int i = 0; rc && (i < n); i++) {
    rc = (x[i] == x1[i]);
  }

  return rc;
}

void NLOptWrFCArgs::setXVec(unsigned int n, const double *x1) {
  for (unsigned int i = 0; i < n; i++)
    x[i] = x1[i];
  // x.assign(x1, x1 + n);

  for (size_t j = 0; j < numThreads; j++) {
    fArgs[j].xTmp1 = x;
    fArgs[j].xTmp2 = x;
  }
}

// ===================================================================
// ===================================================================

void NLOptWrFCArgs::reCalculateIfNecessary(unsigned int n, const double *x1,
                                           bool hasGrad) {
  bool toRecalc = hasConstraints();
  if (!toRecalc)
    toRecalc = !checkXVec(n, x1);

  if (toRecalc) {
    setXVec(n, x1);

    toRecalculateGradients = true;

    // 1.) function evaluation with constraints (single threaded)
    optFknBases[0]->optFktn(x, fc);
  }

  // lazy evaluation:
  if (toRecalculateGradients && hasGrad) {
    // 2. calculation of gradients (multi threaded)

#pragma omp parallel default(shared) num_threads(numThreads)
    {
#pragma omp for schedule(static)
      for (long int i = 0; i < xDim; i++) {
        size_t jj = utl::OmpHelper::getThreadNum();
        FArg &fArgSRef = fArgs[jj];

        double dx = difX[i];

        // store value
        double xOld = fArgSRef.xTmp1[i];

        fArgSRef.xTmp2[i] = (xOld + dx);
        fArgSRef.xTmp1[i] = (xOld - dx);

        // function calls
        optFknBases[jj]->optFktn(fArgSRef.xTmp2, fArgSRef.fcTmp2);
        optFknBases[jj]->optFktn(fArgSRef.xTmp1, fArgSRef.fcTmp1);

        for (size_t j = 0; j < fcArgSize; j++) {
          fcGradients[i][j] =
              (fArgSRef.fcTmp2[j] - fArgSRef.fcTmp1[j]) * difXR[i];
        }

        // restore values
        fArgSRef.xTmp2[i] = xOld;
        fArgSRef.xTmp1[i] = xOld;
      }
    }
    toRecalculateGradients = false;
  }
}

// ===================================================================
// ===================================================================

double NLOptWrFCArgs::f(unsigned n, const double *x1, double *fGradVal) {
  bool useGradient = (fGradVal != nullptr);

  reCalculateIfNecessary(n, x1, useGradient);

  if (useGradient) {
    for (size_t i = 0; i < xDim; i++) {
      fGradVal[i] = fcGradients[i][0]; // TODO
    }
  }

  return fc[0];
}

// ===================================================================
// ===================================================================

void NLOptWrFCArgs::multiEqCconstraint(unsigned int m, double *c,
                                       unsigned int n, const double *x1,
                                       double *cGrad) {

  bool useGradient = (cGrad != nullptr);

  reCalculateIfNecessary(n, x1, useGradient);

  for (size_t i = fArgPositions[POS_CEQ], j = 0; i < fArgPositions[POS_CNE];
       i++, j++) {
    c[j] = fc[i];
  }

  if (useGradient) {
    for (size_t i = fArgPositions[POS_CEQ], j = 0; i < fArgPositions[POS_CNE];
         i++, j++) {
      for (size_t k = 0; k < xDim; k++) {
        cGrad[j * xDim + k] = fcGradients[k][i];
      }
    }
  }
}

// ===================================================================
// ===================================================================

void NLOptWrFCArgs::multiNeCconstraint(unsigned int m, double *c,
                                       unsigned int n, const double *x1,
                                       double *cGrad) {
  bool useGradient = (cGrad != nullptr);

  reCalculateIfNecessary(n, x1, useGradient);

  for (size_t i = fArgPositions[POS_CNE], j = 0; i < fArgPositions[POS_LAST];
       i++, j++) {
    c[j] = fc[i];
  }

  if (useGradient) {
    for (size_t i = fArgPositions[POS_CNE], j = 0; i < fArgPositions[POS_LAST];
         i++, j++) {
      for (size_t k = 0; k < xDim; k++) {
        cGrad[j * xDim + k] = fcGradients[k][i];
      }
    }
  }
}

// ===================================================================
// ===================================================================

} // namespace nloptwr
