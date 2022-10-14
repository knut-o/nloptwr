
#ifndef XH_H
#define XH_H

namespace opttest {

/**
 * definition of a point
 * @param x value of horizontal coordinate
 * @param h value of vertical coordinate
 */
class XH {
public:
  /**
   * constructor
   * @param xVal horizontal coordinate
   * @param hVal vertical coordinate
   */
  XH(double xVal = 0.0, double hVal = 0.0);

  /// destructor
  ~XH();

  /// x = horizontal coordinate
  double x;

  /// h = vertical coordinate
  double h;
};

} // namespace opttest

#endif // XH_H
