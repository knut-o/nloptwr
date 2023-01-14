
#ifndef O_PROBLEM_H
#define O_PROBLEM_H

#include "nloptwr/optfktnbase.h"
#include "nloptwr/optfktnclass.h"

#include <functional>
#include <memory>
#include <vector>

namespace opttest {

/**
 * @class OProblem
 * A simple optimization task
 * (implementation of test function)
 */
class OProblem : public oif::OptFknClass {

public:
  /**
   * constructor
   * @param dim dimension
   */
  OProblem(int dim);

  /// destructor
  virtual ~OProblem();

  /**
   * implementation of function definition
   * @param x vector of arguments
   * @param cf vector of target function f, equality constraints (c_i = 0.0), and inequality constraints (c_i <= 0.0)
   */
  virtual void optFktn(const std::vector<double> &x,
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

private:
  /// dimension of x-vector
  int nDim;

  /// number of inequality constraints
  const static int mNeDim = 2;

  /// internally used constants
  const double constraint_data[mNeDim][2] = {
      {2.0, 0.0},
      {-1.0, 1.0},
  };
};

} // namespace opttest

#endif // O_PROBLEM_H
