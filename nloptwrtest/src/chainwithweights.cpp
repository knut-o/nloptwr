
#include "nloptwrtest/chainwithweights.h"

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

ChainWithWeights::ChainWithWeights(int dim, double xN, double hN, double lM,
                                   double lI,
                                   const std::vector<double> &myWeights)
    : oif::OptFknClass(), nDim(dim), x0(0.0), h0(0.0), xn(xN), hn(hN), lm(lM),
      li(lI), weights(dim + 2, 0.0), xc(dim + 2, 0.0), hc(dim + 2, 0.0) {
  for (size_t i = 0; (i < dim) && (i < myWeights.size()); i++) {
    weights[i + 1] = myWeights.at(i);
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
  hc[0] = h0;
  xc[0] = x0;
  xc[nDim + 1] = xn;
  hc[nDim + 1] = hn;

  // calculate positions of chain links (between two ends)
  for (size_t i = 0; i < nDim; i++) {
    xc[i + 1] = xc[i] + li * cos(angleInit);
    hc[i + 1] = hc[i] + li * sin(angleInit);
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
  // lefe side
  hc[0] = h0;
  xc[0] = x0;

  xc[nDim + 1] = xn;
  hc[nDim + 1] = hn;

  // reset constraints
  for (size_t i = 0; i < mDim; i++)
    c[i] = 0.0;

  // copy variable x[i] into hc[i]
  for (size_t i = 0; i < nDim; i++) {
    xc[i + 1] = xc[i] + li * cos(x[i]);
    hc[i + 1] = hc[i] + li * sin(x[i]);
  }

  double diffX = (xc[nDim] - xn);
  // equality constraint as two inequality constraints
  double diffH = (hc[nDim] - hn);
  // squared difference of length of chain link
  double diffL = (diffX * diffX + diffH * diffH) - li * li;

  // equality constraint has two inequality constraints
  c[0] = diffL;
  c[1] = 0.0 - diffL;

  // additionally nonequal constraints
  c[2] = (diffX > 0.0) ? diffX * diffX : 0.0;
  c[2] = (diffH > 0.0) ? diffH * diffH : 0.0;

  // W_pot/g => m*h => min
  double w_g = 0.0;
  double w_g_chain = 0.0;
  double w_g_weights = 0.0;

  // chain
  for (size_t i = 0; i <= nDim; i++) {
    w_g_chain += (hc[i] + hc[i + 1]) * 0.5;
  }
  w_g_chain *= (lm * li);

  // external weights
  for (size_t i = 1; i <= nDim; i++) {
    w_g_weights += hc[i] * weights[i];
  }

  w_g = (w_g_chain + w_g_weights);

  return w_g;
}

void ChainWithWeights::printResult(const std::vector<double> &x,
                                   std::ostream &os) {

  vector<double> c(mDim, 0.0);
  double optVal = optFktn(x, c);

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

  os << "# i, xc, hc " << endl;
  for (size_t i = 0; i <= (nDim + 1); i++) {
    os << setw(3) << i << " " << setw(7) << xc[i] << " " << setw(7) << hc[i]
       << endl;
  }
  os << endl;
}

} // namespace opttest
