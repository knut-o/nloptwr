
#ifndef CHAIN_WITH_WEIGHTS_H
#define CHAIN_WITH_WEIGHTS_H

#include "nloptwr/nloptwr.h"
#include "nloptwr/optfktnbase.h"
#include "nloptwr/optfktnclass.h"

#include "nloptwrtest/xh.h"

#include "nloptwrtest/groundif.h"

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace opttest {

/*
 *
 *        h ^                            + xhn=xh[nDim+1].h
 *          |                           /|
 *          |                          / |
 *          |                         /  |
 *          |                        +   |
 *          |                       /#   |
 *          |                      /     |
 *          |                     /      |
 *          |                    / ...   |
 *      h0  +                   +        |
 * =hc[0].h | \                /#        |
 *          |  \              / w3       |
 *          |   \ xh[1]      /           |
 *          |    \    xh[2] /            |
 *          |     +-------+              |
 *          |     #       #              |
 *          |    w1       w2             |
 *          |                            |
 *          |                            |
 *          +-----+-------+-----+--------+
 *         x0    xc[1]   xc[2] xc[3] ... xn
 *     =xh[0].x                         =xh[nDim+1].x
 *
 *  nDim = number of free paramters (angles)
 *  w1, w2, ... = additional weights
 *  h = height
 *  xh[i].h = height at position
 *  xh[i].x = coordinate x at position
 *
 */

/**
 * @class ChainWithWeights
 * A simple optimization task
 * (implementation of test function)
 */
class ChainWithWeights : public oif::OptFknClass {

public:
  /**
   * constructor
   * @param dim dimension
   * @param xn  x-value on the right corner
   * @param hn  height on the right corner
   * @param lm  mass per length of chain (without separate weights)
   * @param li  length of a chain link
   * @param weights separate weights at pos (dim=n-1, pos=0 => i=1, ...)
   */
  ChainWithWeights(
      int dim,
      const XH &x_h = XH(1000, 250.0), // double xN = 1000, double hN = 250,
      double lM = 1, double lI = 1,
      const std::vector<double> &myWeights = std::vector<double>(),
      GroundIf *my_Ground = nullptr);

  /// destructor
  virtual ~ChainWithWeights();

  /**
   * implementation of function definition
   * @param x (vector of) function arguments
   * @param c vector of nonequal constraints (c_i <= 0.0)
   * @return value of target function
   */
  virtual double optFktn(const std::vector<double> &x,
                         std::vector<double> &c) override;

  /**
   * initialize method
   * @param lb lower bounds
   * @param ub upper bounds
   * @param xInit initial value for all arguments
   */
  virtual void initialize(); //  double lb, double ub, double xInit );

  /**
   * clone method
   * @return reference of clone (of this object)
   */
  virtual oif::OptFknBase *clone() const override;

  /**
   * print result
   * @param x arguments
   * @param os output stream
   */
  void printResult(const std::vector<double> &x, std::ostream &os);

private:
  /// dimension of x-vector
  int nDim;

  /// number of last index = number of chain links = (nDim+1)
  int noOfChainLinks;

  /// left corner
  XH xh0;

  /// right corner
  XH xhn;

  /// mass per length
  double lm;

  /// length of a chain link
  double li;

  /// external weights
  std::vector<double> weights;

  // vector of coordinates (x,h)
  std::vector<XH> xh;

  // epsilon
  static const double epsilon;

  // PI
  static const double PI;

  /// number of nonequal constraints
  int mDim;

  /// definition of ground
  std::shared_ptr<GroundIf> myGround;

  /// the number of nonequal constraints to express eqality constraints
  int offSetByEqualConstraints;

  /// calculate non-equality constraints (internaly used by method "optFktn")
  void catculateNEConstraints(std::vector<double> &c1);

  /// the maximal step size of x
  double dxMaxGround;
};

} // namespace opttest

#endif // CHAIN_WITH_WEIGHTS_H
