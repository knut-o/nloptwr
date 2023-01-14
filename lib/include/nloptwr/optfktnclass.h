

#ifndef OPT_FKTN_CLASS_H
#define OPT_FKTN_CLASS_H

#include <functional>
#include <memory>
#include <nlopt.hpp>
#include <vector>

#include "optfktnbase.h"

namespace oif {

/**
 * @class OptFknClass
 * This class is a specialization of OptFknBase
 */
class OptFknClass : public OptFknBase {

public:
  /// constructor
  OptFknClass();

  /// destructor
  virtual ~OptFknClass();

  /**
   * (abstract) target function
   * @param x vector of arguments
   * @param cf vector of target function f, equality constraints (c_i = 0.0),
   * and inequality constraints (c_i <= 0.0)
   */
  virtual void optFktn(const std::vector<double> &x,
                       std::vector<double> &fc) = 0;

  /**
   * get number of arguments x
   * @return number of arguments x
   */
  virtual size_t getSizeOfX() const;

  /**
   * get number of equality constraints
   * @return number of equalityints
   */
  virtual size_t getSizeOfEqConstraints() const;

  /**
   * get number of inequality constraints
   * @return number of inequality constraints
   */
  virtual size_t getSizeOfNeConstraints() const;

  /**
   * get upper bounds
   * @return vector of lower bounds
   */
  virtual std::vector<double> getUpperBounds() const;

  /**
   * get lower bounds
   * @return vector of lower bounds
   */
  virtual std::vector<double> getLowerBounds() const;

  /**
   * get initial argument values
   * @return vector of initial arguments
   */
  virtual std::vector<double> getXInitial() const;

  /**
   * get function objectOPT_FKTN_BASE2_H
   * @return function object @see optFunction
   */
  optFunction getOptFunktion();

  /**
   * (abstract) clone method
   * @return reference of derived object
   */
  virtual OptFknBase *clone() const = 0;

  /**
   * validation means range checks
   * For all indices i is:
   * lB[i] <= xInitial[i] <= uB[i]
   *
   * @return true if is everything OK.
   */
  virtual bool validate() const;

  // protected:

  /**
   * initialization method
   * This method has to be called before use of the object.
   * @param dim dimension of arguments vector (x)
   * @param nrOfEqConstraints number of equality constraints
   * @param nrOfNeConstraints number of inequality constraints
   * @param lb lower bounds for all values
   * @param ub upper bounds for all values
   * @param xInit initial value for all arguments (x)
   */
  virtual void init(size_t dim, size_t nrOfEqConstraints,
                    size_t nrOfNeConstraints, double lb, double ub,
                    double xInit);

  /**
   * set initial value of x
   * @param idx index
   * @param val value
   */
  void setXInit(std::size_t idx, double val);

  /**
   * set a lower bound at idx
   * @param idx index
   * @param val value
   */
  void setLb(std::size_t idx, double val);

  /**
   * set a upper bound at idx
   * @param idx index
   * @param val value
   */
  void setUb(std::size_t idx, double val);

private:
  /// number of parameters
  size_t nDim;

  /// number of parameters
  // size_t mDim;

  /// number of equality constraints
  size_t mDimEq;

  /// number od inequality constrains
  size_t mDimNe;

  /// total number of elements of vectoe fc
  size_t fcSize;

protected:
  /// lower boundsOPT_FKTN_BASE2_H
  std::shared_ptr<std::vector<double>> lB;

  /// upper bounds
  std::shared_ptr<std::vector<double>> uB;

  /// upper bounds
  std::shared_ptr<std::vector<double>> xInitial;
};

} // namespace oif

#endif // OPT_FKTN_CLASS_H
