
#ifndef NLOPT_WR_FCARGS_H
#define NLOPT_WR_FCARGS_H

#include "nloptwr/optfktnbase.h"
#include <memory>
#include <vector>

namespace nloptwr {

struct FArg {

public:
  /**
   * constructor
   * @param xDim dimension of function arguments
   * @param cEqSize number of equality constraints
   * @param cNeSize number of inequality constraints
   */
  FArg(size_t xDim1, size_t fCSize);

  /// deleted defaulr constructor
  FArg() = delete;

  /// destructor
  ~FArg();

  /// size of fcArg
  size_t fcSize;

  /// function arguments
  std::vector<double> xTmp1;

  /// 1st FArg
  std::vector<double> fcTmp1;

  /// function arguments
  std::vector<double> xTmp2;

  /// 2nd argument
  std::vector<double> fcTmp2;
};

/**
 * class to manage different structures of NLoptWr
 */
class NLOptWrFCArgs {

public:
  /// default constructor
  NLOptWrFCArgs();

  /// destructor
  ~NLOptWrFCArgs();

  /**
   * initialization method
   * @param container with implementation of target functions
   * @param differences for difference quotient
   */
  void init(std::vector<std::shared_ptr<oif::OptFknBase>> &optFkn,
            const std::vector<double> &dX);

  /**
   * internally used function
   * @param n dimension of x
   * @param x argument vector (c-array)
   * @param fGradVal array vector of function value (array)
   * @return function value
   */
  double f(unsigned n, const double *x1, double *fGradVal);

  /**
   * internally used equality constraints function
   * @param m number of constraints
   * @param c vector of single constraint function result values
   * @param n number of optimization parameters
   * @param x optimization parameters
   * @param cGrad gradient of constraints (array of size m*n)
   */
  void multiEqCconstraint(unsigned int m, double *c, unsigned int n,
                          const double *x1, double *cGrad);

  /**
   * internally used inequality constraints function
   * @param m number of constraints
   * @param c vector of single constraint function result values
   * @param n number of optimization parameters
   * @param x optimization parameters
   * @param cGrad gradient of constraints (array of size m*n)
   */
  void multiNeCconstraint(unsigned int m, double *c, unsigned int n,
                          const double *x1, double *cGrad);

  /**
   * check equality constraints
   * @return true if equality constraints are existing, otherwise false
   */
  bool hasEqConstraints() const;

  /**
   * check inequality constraints
   * @return true if inequality constraints are existing, otherwise false
   */
  bool hasNeConstraints() const;

  /// has constraints
  bool hasConstraints() const { return ((noOfEqConstraints > 0) || (noOfEqConstraints > 0)); }

private:
  /**
   * @param n dimension of arguments
   * @param x1 arguments
   * @return true if a update is necessary
   */
  bool checkXVec(unsigned int n, const double *x1) const;

  /**
   * @param n dimension of arguments
   * @param x1 arguments
   */
  void setXVec(unsigned int n, const double *x1);

  /**
   * recalculate if necessary
   * @param n dimension of arguments
   * @param x1 function arguments
   */
  void reCalculateIfNecessary(unsigned int n, const double *x1, bool hasGrad);

  /// object of parallel calculation instances
  std::vector<std::shared_ptr<oif::OptFknBase>> optFknBases;

  /// reziproke differences * 0.5
  std::vector<double> difX;

  /// reziproke differences * 0.5
  std::vector<double> difXR;

  // number of threads
  int numThreads;

  /// number of arguments x
  size_t xDim;

  /// size of array =(1+xDim)
  size_t arraySize;

  /// number of equality constraints
  size_t noOfEqConstraints;

  /// number of inequality constraints
  size_t noOfNeConstraints;

  /// total number constraints
  size_t noOfConstraints;

  /// total number of elements (=1+noOfEqConstraints+noOfNeConstraints)
  size_t fcArgSize;

  /// vector of positions
  std::vector<size_t> fArgPositions;

  /// function arguments
  std::vector<double> x;

  /// vector of functin evaluation (function and constraints)
  std::vector<double> fc;

  /// array with derivations and the fuchtion evaluation
  std::vector<std::vector<double>> fcGradients;

  // =====================================================

  /// temporary storage (threads)
  std::vector<FArg> fArgs;
  
  /// recalculate gradients flag
  bool toRecalculateGradients;

private:
  // =====================================================

  /// ist position of fcArgs of noequal constraint
  static const size_t POS_CEQ = 1;

  /// ist position of fcArgs of target function value
  static const size_t POS_CNE = 2;

  /// ist position behind of fcArgs
  static const size_t POS_LAST = 3;
};

} // namespace nloptwr

#endif // NLOPT_WR_FCARGS_H
