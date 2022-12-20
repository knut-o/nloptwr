
#ifndef GROUND_LIN_H
#define GROUND_LIN_H

#include "nloptwrtest/groundif.h"
#include "nloptwrtest/xh.h"

namespace opttest {

class GroundLin : public GroundIf {

public:
  /**
   * Linear ground
   * @param p_1 1st point
   * @param p_2 2nd point
   * @param d_x tolerance of x
   */
  GroundLin(const XH &p_1 = XH(0.0, -1.0E12), const XH &p_2 = XH(1.0, -1.0E12),
            double d_x = 1.0E5);

  /// destructor
  ~GroundLin();

  /**
   * definition of the ground
   * @param x position x
   * @return height of ground
   */
  virtual double ground(double x);

  /**
   * get minimal distance of points of interval
   * @return minimal distance
   */
  virtual double getDx();

  /**
   * get if of object
   * @return id
   */
  virtual int getId();

  /**
   * clone operator
   * @return GroundIf Object
   */
  virtual GroundIf *clone();

private:
  /// 1st point
  XH p1;

  // 2nd point
  XH p2;

  /// slope
  double b;

  /// step size
  double dx;
};

} // namespace opttest

#endif // GROUND_LIN_H
