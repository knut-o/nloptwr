
#ifndef NLOPT_WR_H
#define NLOPT_WR_H

#include "nloptwralgorithm.h"
#include "nloptwrsearchalgoparam.h"
#include "optfktnbase.h"

namespace utilx {
class LRegX;
}

#include <functional>
#include <memory>
#include <nlopt.hpp>
#include <vector>

namespace nloptwr {

/**
 * Function arguments
 * @param f return value of function
 * @param x function arguments
 * @param c constraints
 */
struct FArg {
public:
  /// function value
  /// constructor
  FArg();

  ///  destructor
  ~FArg();

  /// function value
  double f;

  /// function arguments
  std::vector<double> x;

  /// constraints
  std::vector<double> c;
};

class NLOptWrParamFactory;

class NLOptWrSStrat;

/**
 * This is a wrapper for nlopt
 * for optimization methods without gradients
 */
class NLOptWrapper {

public:
  /** constructor
   * @param oFktn reference to function evaluation object
   * @param nThr number of threads (0=default)
   */
  NLOptWrapper(const oif::OptFknBase &oFktn, unsigned int nThr = 0);

  /**
   * destructor
   */
  ~NLOptWrapper();

  /** calculation of initial steps from lower bounds, and upper bounds
   * @param numberOfSteps (default: 15)
   */
  void calculateInitialStep(double numberOfSteps = 15);

  /**
   * get selected algorithms
   * @param nloptWrSStrat selection criteria
   * @return selected algorithm objects
   */
  std::vector<NLOptWrAlgorithm>
  getSelectedAlgorithms(const NLOptWrSStrat &nloptWrSStrat) const;

  /**
   * set search minimum
   * @param sMin flag (true means minimum, false mean maximum)
   */
  void setSearchMin(bool sMin = true);

  /** optimization call
   * @param fOpt return value of function object
   * @param nloptWrSStrat ssearch strategy
   * @param maxTime set max time in seconds (default: 3600)
   * @param maxEval set max evaluations (default: 1000000)
   */
  nlopt::result optimize(const NLOptWrSStrat &nloptWrSStrat, int maxTime = 3600,
                         int maxEval = 1000000);

  /**
   * get optimized value
   */
  double getLastOptimumValue() const;

  /**
   * get search flag
   * @return search flag: true means minimum, false mean maximum
   */
  bool getSearchMin() const;

  /** get result vector
   * @return vector of arguments
   */
  const std::vector<double> &getX() const;

  /** get text of return value (nlopt::result)
   * @return c-string of result value
   */
  static const char *getStringOfResult(nlopt::result r);

  /** set difference of differential quotient
   * @param val value (default: 1e-7)
   */
  void setDx(double val = 1.0E-7);

  /**
   * set difference of differential quotient
   * @param vals difference for differential quotients
   */
  void setDx(const std::vector<double> &vals);

  /**
   * get the solution (target value) */
  double getFOpt();

  // ------------------------------------------------------------------------------------------------

  /**
   * ask for preferred algorithm
   * @param p4pAlg search parameter
   * @return flag (true if a preferred algorithm exists)
   */
  bool hasPreferedAlgorithms() const;

  /**
   * delete all preferred algorithms
   */
  void deletePreferedAlgorithms();

  /**
   * set a preferred algorithm
   * @param algx enum of algorithm
   * @param xDimMin minimal size of arguments (<=0 means unchanged)
   * @param xDimMax maximal number of arguments (0 means unlimited)
   * @return flag (true if a preferred algorithm is deleted)
   */
  void setPreferedAlgorithm(nlopt::algorithm algx, int xDimMin = -1,
                            int xDimMax = 0);

  /**
   * set a preferred algorithm
   * @param algxs enums of algorithms
   * @return flag (true if a preferred algorithm is deleted)
   */
  void setPreferedAlgorithms(const std::vector<nlopt::algorithm> &algxs);

  /**
   * get preferred algorithms
   * @return flag (true if a preferred algorithm is deleted)
   */
  const std::vector<NLOptWrAlgorithm> &getPreferedAlgorithms() const;

  // ------------------------------------------------------------------------------------------------

  /**
   * get population size (for stochastic algorithms)
   */
  unsigned int getPopulation() const;

  /**
   * set population size (for stochastic algorithms)
   */
  void setPopulation(unsigned int val = 0);

  /**
   * set vector storage for some vector based methods (i.e. LBFGS)
   * @param val number data sets
   */
  void setVectorStorage(unsigned int val = 10);

  /**
   * get vector storage for some vector based methods (i.e. LBFGS)
   * @return number data sets
   */
  unsigned int getVectorStorage() const;

  // ------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------

  /**
   * get time of previous optimization
   * @return time of optimization in seconds
   */
  double getOptTime() const;

  // ------------------------------------------------------------------------------------------------

  /**
   * set tolerance of (no equal) nonlinear constraints
   * @param val tolerance for all nonlinear constraints (default: 1e-10)
   */
  void setTolMConstraints(double val = 1.0E-10);

  /**
   * set tolerance of (no equal) nonlinear constraints
   * @param vals tolerances for nonlinear constraints
   */
  void setTolMConstraints(const std::vector<double> &vals);

  // ------------------------------------------------------------------------------------------------

  /// set all reduction factors of subopts
  void setSubFactors();

  /// set all reduction factors of subopts with the same value
  void setSubFactors(double val = 0.1);

  // ------------------------------------------------------------------------------------------------

  /// set the reduction factor of absolute function tolerance of subopt
  void setSubFTolAbsFactor(double val = 0.1);

  /// get the reduction factor of absolute function tolerance of subopt
  double getSubFTolAbsFactor() const;

  /// set the reduction factor of relative function tolerance of subopt
  void setSubFTolRelFactor(double val = 0.1);

  /// get the reduction factor of relative function tolerance of subopt
  double getSubFTolRelFactor() const;

  /// set the eduction factor of relative arguments tolerance of subopt
  void setSubXtolRelFactor(double val = 0.1);

  /// set the eduction factor of relative arguments tolerance of subopt
  double getSubXtolRelFactor() const;

  /// set the reduction factor of absolute arguments tolerance of subopt
  void setSubXTolAbsFactor(double val = 0.1);

  /// set the reduction factor of absolute arguments tolerance of subopt
  double getSubXTolAbsFactor() const;

  // ------------------------------------------------------------------------------------------------

  /// set absolute tolerance of function value
  void setFTolAbs(double val = 1.0E-10);

  /// get absolute tolerance of function value
  double getFTolAbs() const;

  /// get absolute tolerance of function value
  double getFTolAbsSubOpt() const;

  /// set absolute tolerance of function value
  void setFTolRel(double val = 1.0E-11);

  /// set absolute tolerance of function value
  double getFTolRel() const;

  /// set absolute tolerance of function value
  double getFTolRelSubOpt() const;

  /**
   * set tolerance
   * @param val value (default: 1e-9)
   */
  void setXTolAbs(double val = 1.0E-7);

  /**
   * set tolerance
   * @param vals vector of tolerances
   */
  void setXTolAbs(const std::vector<double> &vals);

  /**
   * get tolerance
   * @return vals vector of tolerances
   */
  const std::vector<double> &getXTolAbs() const;

  /**
   * get absolute tolerance of sub optimization
   * @return vals vector of tolerances
   */
  std::vector<double> getXTolAbsSubOpt() const;

  /**
   * set relative tolerance
   * @param val tolerance (for all elements)
   */
  void setXTolRel(double val = 1E-10);

  /**
   * get relative tolerance
   * @return val tolerance (for all elements)
   */
  double getXTolRel() const;

  /**
   * get relative tolerance
   * @return val tolerance (for all elements) of subalgorithm
   */
  double getXTolRelSubOpt() const;

  /**
   * get number of threads
   */
  size_t getNThreads() const;

private:
  /** target functions
   * This are clones of optFknBase
   * @return clones of optFknBase
   */
  std::vector<std::shared_ptr<oif::OptFknBase>> optFknBases;

  /// number of dimensions (x)
  long int nDim;

  /// number of constraints
  long int mDim;

  /// pointer to the optimization object
  std::shared_ptr<nlopt::opt> opt;

  /// parameters to be optimized
  std::vector<double> x;

  /// upper bounds
  std::vector<double> ub;

  /// lower bounds
  std::vector<double> lb;

  // ========================================

  /// number of threads
  size_t nThreads;

  /// difference of differential quotients
  std::vector<double> dX;

  // ========================================

  /// tolerance
  std::vector<double> tolMConstraints;

  // ========================================

  /// initial step
  std::vector<double> initial_step;

  /// initial step of subalgorithm
  std::vector<double> initialSubStep;

  // ========================================

  /// reduction factor of absolute function tolerance of subopt (lower than 1)
  double subFTolAbsFactor;

  /// reduction factor of relative function tolerance of subopt (lower than 1)
  double subFTolRelFactor;

  /// eduction factor of relative arguments tolerance of subopt (lower than 1)
  double subXtolRelFactor;

  /// eduction factor of absolute arguments tolerance of subopt (lower than 1)
  double subXTolAbsFactor;

  // ========================================
  /// tolerance
  double fTolAbs;

  /// tolerance for subopt
  // double fTolAbsSubOpt;

  /// tolerance
  double fTolRel;

  /// rel. tolerance
  double xTolRel;

  /// rel. tolerance
  // double xTolRelSubOpt;

  /// tolerance
  std::vector<double> xTolAbs;

  /// the minimum objective value, upon return
  double fOpt;

  // =======================================

protected:
  /// cached function arguments, values, constraints
  FArg fArgs;

  /// function arguments (Threads)
  std::vector<FArg> fArgs1;

  /// function arguments (Threads)
  std::vector<FArg> fArgs2;

  /// gradient of function (nDim)
  std::vector<double> fGrad;

  /// vector gradient of single constraints (mDim*nDim)
  std::vector<std::vector<double>> cGrad;

  /// gradient flag
  bool useGradient;

  // =======================================
private:
  /**
   * this function compares the value of x1 with fArgs.x
   * @param n  the size of x1
   * @param x1 new array x1
   * @return true if x1 and fArgs.x are identical, otherwise false
   */
  bool checkXVec(unsigned int n, const double *x1) const;

  /**
   * set fArgs.x vector with x1 if needed
   * method checkXVec is called
   * @param n  the size of x1
   * @param x1 new array x1
   * @return true if fArgs.x is set, otherwise false
   */
  bool setXVec(unsigned int n, const double *x1);

  /** internally used check method (dimension check of vectors)
   * @return success of check (true is OK)
   */
  bool checkVectors() const;

  /**
   * internally used function
   * @param n dimension of x
   * @param x argument vector (c-array)
   * @param fGradVal array vector of function value (array)
   * @return function value
   */
  double f(unsigned n, const double *x, double *fGradVal);

  /**
   * internally used constraints function
   * @param m number of constraints
   * @param c vector of single constraint function result values
   * @param n number of optimization parameters
   * @param x optimization parameters
   * @param cGrad gradient of constraints (array of size m*n)
   */
  void multi_constraint(unsigned int m, double *c, unsigned int n,
                        const double *x, double *cGrad);

protected:
  /**
   * factory class to select algorithms
   */
  std::shared_ptr<NLOptWrParamFactory> nlOptParamFactory;

  /**
   * vector storage for some vector based methods (i.e. LBFGS)
   */
  unsigned int vectorStorage;

  /**
   * method of derivations
   */
  bool useNewDerivMethod;

  /**
   * dimension of linear regression
   */
  std::size_t derivRegrDim;

private:
  /**
   * time of previous optimization
   */
  double optTime;

  /// search minimum flag
  bool searchMin;

  /// population size (for stochastic algorithms)
  unsigned int population;
};

} // namespace nloptwr

#endif // NLOPT_WR_H
