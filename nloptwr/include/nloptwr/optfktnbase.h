

#ifndef OPT_FKTN_BASE_H
#define OPT_FKTN_BASE_H

#include <functional>
#include <memory>
#include <nlopt.hpp>
#include <vector>

namespace oif {

/**
 * optimization function object
 */
typedef std::function<double(const std::vector<double> &,
                             std::vector<double> &)>
    optFunction;

/**
 * interface class of optimization interface
 */
class OptFknBase {

public:
  /// constructor
  OptFknBase();

  /// destructor
  virtual ~OptFknBase();

  /**
   * (abstract) target function
   * @param x vector of arguments
   * @param c vector of nonequal constraints (c_i <= 0.0)
   * @return value of target function
   */
  virtual double optFktn(const std::vector<double> &x,
                         std::vector<double> &c) = 0;

  /**
   * (abstract) target function
   * @param x vector of arguments
   * @param c vector of nonequal constraints (c_i <= 0.0)
   * @return value of target function
   */
  virtual double optF(const std::vector<double> &x) = 0;

  /**
   * (abstract) target function
   * @param x vector of arguments
   * @param c vector of nonequal constraints (c_i <= 0.0)
   * @return value of target function
   */
  virtual void optC(const std::vector<double> &x, std::vector<double> &c) = 0;

  /**
   * (abstract) get number of arguments x
   * @return number of arguments x
   */
  virtual size_t getSizeOfX() const = 0;

  /**
   * get number of nonequal constraints
   * @return number of nonequal constraints
   */
  virtual size_t getSizeOfNeConstraints() const = 0;

  /**
   * (abstract) get upper bounds
   * @return vector of upper bounds
   */
  virtual std::vector<double> getUpperBounds() const = 0;

  /**
   *  (abstract) get lower bounds
   * @return vector of lower bounds
   */
  virtual std::vector<double> getLowerBounds() const = 0;

  /**
   *  (abstract) get initial argument values
   * @return vector of initial arguments
   */
  virtual std::vector<double> getXInitial() const = 0;

  /**
   * get (empty) vector of contraints
   */
  std::vector<double> getZeroNeConstraintVec();

  /**
   * get function object
   * @return
   */
  optFunction getOptFunktion();

  /**
   * definion of clone interface
   * @return instance of derived object
   */
  virtual OptFknBase *clone() const = 0;

  /**
   * validation means range checks
   * For all indices i is:
   * lB[i] <= xInitial[i] <= uB[i]
   *
   * @return true if is everything OK.
   */
  virtual bool validate() const = 0;

private:
  /// instance of optimization function object
  optFunction myOptFunction;
};

} // namespace oif

#endif // OPT_FKTN_BASE_H
