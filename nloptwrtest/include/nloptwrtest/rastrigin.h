
#ifndef RASTRIGIN_H
#define RASTRIGIN_H

#include "nloptwr/optfktnbase.h"
#include "nloptwr/optfktnclass.h"

#include <functional>
#include <memory>
#include <vector>

namespace opttest {

/**
 * @class Rastrigin
 * A simple optimization task
 * (implementation of Rastrigin test function)
 */
class Rastrigin : public oif::OptFknClass {

public:
  /**
   * constructor
   * @param dim dimension
   */
  Rastrigin(int dim);

  /// destructor
  virtual ~Rastrigin();

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
  virtual void initialize(double lb = -10.0, double ub = 10.0,
                          double xInit = 7.0);

  /**
   * clone method
   * @return reference of clone (of this object)
   */
  virtual oif::OptFknBase *clone() const override;

private:
  /// dimension of x-vector
  int nDim;

  ///  constant PI/2
  static const double Pi2;
};

} // namespace opttest

#endif // RASTRIGIN_H
