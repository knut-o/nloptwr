
#ifndef GRIEWANK_H
#define GRIEWANK_H

#include "nloptwr/optfktnbase.h"
#include "nloptwr/optfktnclass.h"

#include <functional>
#include <memory>
#include <vector>

namespace opttest {

/**
 * @class Griewank
 * A simple optimization task
 * (implementation of Griewank test function)
 */
class Griewank : public oif::OptFknClass {

public:
  /**
   * constructor
   * @param dim dimension
   */
  Griewank(int dim);

  /// destructor
  virtual ~Griewank();

  /**
   * implementation of function definition
   * @param x vector of arguments
   * @param cf vector of target function f, equality constraints (c_i = 0.0),
   * and inequality constraints (c_i <= 0.0)
   */
  virtual void optFktn(const std::vector<double> &x,
                       std::vector<double> &fc) override;

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
};

} // namespace opttest

#endif // GRIEWANK_H
