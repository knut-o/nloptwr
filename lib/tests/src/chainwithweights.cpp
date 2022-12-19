
#include "nloptwrtest/chainwithweights.h"
#include "nloptwrtest/groundif.h"
#include "nloptwrtest/xh.h"

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

// static
const double ChainWithWeights::PI = std::atan(1.0) * 4.0;

// static
const double ChainWithWeights::epsilon = 5.0E-6;

ChainWithWeights::ChainWithWeights(int dim, const XH &xhN, double lM, double lI,
                                   const std::vector<double> &myWeights,
                                   GroundIf *my_Ground)
    : oif::OptFknClass(), nDim(dim), noOfChainLinks(dim + 1), xh0(0.0, 0.0),
      xhn(xhN), lm(lM), li(lI), weights(dim + 2, 0.0), xh(dim + 2, 0.0) {

  offSetByEqualConstraints = 2;
  mDim = offSetByEqualConstraints;
  dxMaxGround = 100000.0;

  // if a ground is defined
  if (my_Ground != nullptr) {
    myGround = shared_ptr<GroundIf>((*my_Ground).clone());

    // each chain link section is an additional constraint
    mDim += 2;

    dxMaxGround = abs((*myGround).getDx());
  }

  for (size_t i = 0; (i <= noOfChainLinks) && (i < myWeights.size()); i++) {
    weights[i] = myWeights.at(i);
  }
  initialize();
  validate();
}

// virtual
void ChainWithWeights::initialize() //  double lb, double ub, double xInit )
{
  double angleInit = PI * 0.1;

  init(nDim, mDim, -0.5 * PI, 0.5 * PI, angleInit);

  // left side
  xh[0] = xh0;
  xh[nDim + 1] = xhn;

  // calculate positions of chain links (between two ends)
  for (size_t i = 0; i < nDim; i++) {
    xh[i + 1].x = xh[i].x + lm * cos(angleInit);
    xh[i + 1].h = xh[i].h + lm * sin(angleInit);
  }
}

// virtual
ChainWithWeights::~ChainWithWeights() {}

// The clone function is essential for parallelization
oif::OptFknBase *ChainWithWeights::clone() const {
  return (new ChainWithWeights(*this));
}

// virtual
double ChainWithWeights::optFktn(const std::vector<double> &x,
                                 std::vector<double> &c) {
  // ======================================================
  // initialization
  // ======================================================

  // left side
  xh[0] = xh0;

  // right side
  xh[noOfChainLinks] = xhn;

  // reset constraints
  for (size_t i = 0; i < mDim; i++)
    c[i] = 0.0;

  // ======================================================
  // calculate the coordinate between left and right sides
  // ======================================================

  // copy variable x[i] into hc[i]
  for (size_t i = 0; i < nDim; i++) {
    xh[i + 1].x = xh[i].x + lm * cos(x[i]);
    xh[i + 1].h = xh[i].h + lm * sin(x[i]);
  }

  // ======================================================
  // equality constraint has two inequality constraints
  // the length of the last chain link => li
  // ======================================================

  double diffX = (xh[nDim].x - xhn.x);
  double diffH = (xh[nDim].h - xhn.h);

  // equality constraint as two inequality constraints
  // squared difference of length of chain link
  double diffL = (diffX * diffX + diffH * diffH) - (li * li);

  c[0] = diffL;
  c[1] = 0.0 - diffL;

  // ======================================================
  // inequality constraints if a ground exists
  // ======================================================
  if (myGround != nullptr)
    catculateNEConstraints(c);

  // ======================================================
  // calculate the target function
  // W_pot/g => m*h => min
  // ======================================================
  double w_g = 0.0;
  double w_g_chain = 0.0;
  double w_g_weights = 0.0;

  // chain
  for (size_t i = 0; i <= nDim; i++) {
    w_g_chain += (xh[i].h + xh[i + 1].h) * 0.5;
  }
  w_g_chain *= (lm * li);

  // external weights
  for (size_t i = 1; i <= nDim; i++) {
    w_g_weights += xh[i].h * weights[i];
  }

  w_g = (w_g_chain + w_g_weights);

  return w_g;
}

void ChainWithWeights::catculateNEConstraints(std::vector<double> &c) {

  XH pA;
  XH pB;

  double dXi = 0.0;

  double hAB = 0.0;

  int nCount = 0;
  double deltaX = 0.0;
  double deltaH = 0.0;

  double xTmp = 0.0;
  double tmpH = 0.0;
  double hTmp = 0.0;

  // constraints
  double ci = 0.0;
  double ciA = 0.0;
  double ciTmp = 0.0;

  // ==============================
  pA = xh[0];
  hAB = myGround->ground(pA.x);
  ciA = (hAB - pA.h);

  double ciMaxTotal = 0.0;
  double ciSumGtZero = 0.0;

  for (size_t i = 1; i <= noOfChainLinks; i++) {
    ci = ciA;

    dXi = (pB.x - pA.x);
    if (abs(dXi) > dxMaxGround) {
      nCount = trunc((abs(dXi) > dxMaxGround) / dxMaxGround) + 1;

      deltaX = dXi / static_cast<double>(nCount);
      deltaH = (pB.h - pA.h) / static_cast<double>(nCount);

      xTmp = pA.x;
      for (int j = 1; j < nCount; j++) {
        xTmp += deltaX;

        // constant slope of chain link
        tmpH += deltaH;

        // ground at posiotion xTmp
        hTmp = myGround->ground(xTmp);

        // calculate constraint
        ciTmp = (hTmp - tmpH);

        // calulate maximum
        ci = max(ci, ciTmp);
      }
    }

    // calculate distance at end of chain link
    pB = xh[i];
    hAB = myGround->ground(pB.x);
    ciA = (hAB - pB.h);
    ci = max(ci, ciA);

    ciMaxTotal = max(ci, ciMaxTotal);
    if (ci > 0.0)
      ciSumGtZero += ci * (ci + 1.0);

    // reuse pA and hAB
    pA = pB;
  }

  // store constaints
  c[2] = ciSumGtZero;
  if (mDim > 3)
    c[3] = ciMaxTotal;
}

void ChainWithWeights::printResult(const std::vector<double> &x,
                                   std::ostream &os) {

  vector<double> c(mDim, 0.0);
  double optVal = optFktn(x, c);

  double hMin = 10000000.0;
  if (!myGround) {
    for (size_t i = 0; i <= noOfChainLinks; i++)
      hMin = min(hMin, xh[i].h);
    hMin -= 0.5;
  }

  os << " " << endl;
  os << "# nDim = " << nDim << endl;
  os << "# mDim = " << mDim << endl;
  for (size_t i = 0; i < mDim; i++) {
    os << "# c[" << setw(3) << i << "] = " << c[i] << endl;
  }
  os << "# optVal = " << optVal << endl;
  for (size_t i = 0; i < nDim; i++) {
    os << "# x[" << setw(3) << i << "] = " << x[i] << endl;
  }

  os << "# i, xc, hc, hg " << endl;
  for (size_t i = 0; i <= noOfChainLinks; i++) {
    os << setw(3) << i << " " << setw(7) << xh[i].x << " " << setw(7) << xh[i].h
       << " " << setw(7) << ((myGround) ? myGround->ground(xh[i].x) : hMin)
       << endl;
  }
  os << endl;
}

} // namespace opttest
