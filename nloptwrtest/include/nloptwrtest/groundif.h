
#ifndef GROUND_IF_H
#define GROUND_IF_H

namespace opttest {

/**
 * Interface class to define the ground
 */
class GroundIf {

public:
  /// cnstructor
  GroundIf();

  /// destructor
  virtual ~GroundIf();

  /**
   * definition of the ground
   * @param x position x
   * @return height of ground
   */
  virtual double ground(double x) = 0;

  /**
   * get minimal distance of points of interval
   * @return minimal distance
   */
  virtual double getDx() = 0;

  /**
   * get if of object
   * @return id
   */
  virtual int getId() = 0;

  /**
   * clone operator
   * @return GroundIf Object
   */
  virtual GroundIf *clone() = 0;
};
} // namespace opttest

#endif // GROUND_IF_H
