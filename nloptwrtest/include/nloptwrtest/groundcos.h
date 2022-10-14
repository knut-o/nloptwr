
#ifndef GROUND_COS_H
#define GROUND_COS_H

#include "nloptwrtest/groundif.h"
#include "nloptwrtest/xh.h"

namespace opttest {

class GroundCos : public GroundIf {

public:
  /**
   * linear ground
   * @param p_1 1st point
   * @param p_2 2nd point
   */
  GroundCos(const XH &p_1 = XH(0.0, -1.0E12), const XH &p_2 = XH(1.0, -1.0E12),
            double d_x = 1.0E5);

  /// destructor
  ~GroundCos();

  /**
   * definition of the ground
   * @param x position x
   * @return height of ground
   */
  virtual double ground(double x);

  /**
   * get minimal the distance of points of interval
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

  /// average height
  double a;

  /// amplitude
  double b;

  /// scale factor of x
  double faktor;

  /// step size
  double dx;
};

} // namespace opttest

#endif // GROUND_COS_H
