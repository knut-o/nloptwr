
#ifndef NLOPT_PARAMFACTORY_H
#define NLOPT_PARAMFACTORY_H

#include "nlopt.hpp"
#include "nloptwralgorithm.h"
#include "nloptwrsearchalgoparam.h"
#include "nloptwrsearchalgoparam2.h"
#include "nloptwrsearchalgoparam3.h"

#include <map>
#include <string>
#include <vector>

namespace nloptwr {

class NLOptWrSearchAlgoParam2;

/**
 * This factoy class
 * to create proper parameters for optimization
 */
class NLOptWrParamFactory {

public:
  /// constructor
  NLOptWrParamFactory();

  /// destructor
  ~NLOptWrParamFactory();

  // ------------------------------------------------------------------------------------------------

  /**
   * get a set of agorithms
   * @param p4pAlg search parameter (except dimension of parameter x)
   * @return algorithms
   */
  std::vector<NLOptWrAlgorithm>
  getAlgorithm(const NLOptWrSearchAlgoParam3 &p4pAlg) const;

  /**
   * get a set of agorithms
   * @param searchStrategy search strategy enum
   * @param hasConstraints constraints flag
   * @param useGradient use gradient
   * @param useAugLagBeforeMlsl use Augmented Lagrangian before MLSL
   * @param xDim dimension of parameter x
   */
  std::vector<NLOptWrAlgorithm>
  getAlgorithm(SSTRAT searchStrategy, bool hasConstraints, bool useGradient,
               bool useAugLagBeforeMlsl, std::size_t xDim) const;

  /**
   * get a set of agorithms
   * @param p4pAlg search parameter (except dimension of parameter x)
   * @param xDim dimension of parameter x
   */
  std::vector<NLOptWrAlgorithm>
  getAlgorithm(const NLOptWrSearchAlgoParam2 &p4pAlg, std::size_t xDim) const;

  // ------------------------------------------------------------------------------------------------

  /**
   * get name of algorithm
   * @param alg enum of algorithm
   * @return name of algorthm
   */
  const std::string &getNameOfAlgorithm(nlopt::algorithm alg) const;

  // ------------------------------------------------------------------------------------------------

  /**
   * ask for prefered algorithm
   * @param p4pAlg search parameter
   * @return flag (true if a prefered algorithm exists)
   */
  bool hasPreferedAlgorithms() const;

  /**
   * delete a prefered algorithm
   * @param p4pAlg search parameter
   * @return flag (true if a prefered algorithm is deleted)
   */
  // bool deletePreferedAlgorithm(nlopt::algorithm algx);

  /**
   * delete all prefered algorithms
   */
  void deletePreferedAlgorithms();

  /**
   * set a prefered algorithm
   * @param algx enum of algorithm
   * @param xDimMin minimal number of parameters (-1 means no change)
   * @param xDimMax maximal number of parameters (-1 means no change)
   * @return flag (true if a prefered algorithm is deleted)
   */
  void setPreferedAlgorithm(nlopt::algorithm algx, int xDimMin = -1,
                            int xDimMax = -1);

  /**
   * set a prefered algorithm
   * @param algxs enum of algorithms
   * @return flag (true if a prefered algorithm is deleted)
   */
  void setPreferedAlgorithms(const std::vector<nlopt::algorithm> &algxs);

  /**
   * get prefered algorithms
   * @return flag (true if a prefered algorithm is deleted)
   */
  const std::vector<NLOptWrAlgorithm> &getPreferedAlgorithms() const;

  // ------------------------------------------------------------------------------------------------

  /**
   * get available algorithms
   * @param p4pAlg search parameter
   * @param xDim dimension of parameter x
   * @param ignoreConstraints ignore constraints flag
   * @param searchLocalOrGlobal search local or global flag
   * @param isNotMLSL is not MLSL flag
   * @return availabe algorithms
   */
  std::vector<NLOptWrAlgorithm>
  getAvailableAlgorithms(const NLOptWrSearchAlgoParam &p4pAlg, std::size_t xDim,
                         bool ignoreConstraints = false,
                         bool searchLocalOrGlobal = false,
                         bool isNotMLSL = false) const;

  // ------------------------------------------------------------------------------------------------

protected:
  /// get name of algorithm of index x
  const std::string &getNameOfIdx(size_t idx) const;

  /// get index of algorithm with enum of algorithm
  size_t getIdxOfEnum(nlopt::algorithm) const;

  /// get enum of algorithm with index
  nlopt::algorithm getEnumOfIdx(size_t) const;

  // ------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------

  /**
   * search for stochastic algorithms
   * @param p4pAlg1 search criteria
   * @param xDim number of paramaters (size of x)
   * @param res algorithms that match or noting
   * @return success of search
   */
  bool findSearchAlgorithmR(const NLOptWrSearchAlgoParam &p4pAlg1,
                            std::size_t xDim,
                            std::vector<NLOptWrAlgorithm> &res) const;

  /**
   * search for augmented lagrangian algorithms
   * @param p4pAlg1 search criteria
   * @param xDim number of paramaters (size of x)
   * @param res algorithms that match or noting
   * @return success of search
   */
  bool findSearchAlgorithmAugLag(const NLOptWrSearchAlgoParam &p4pAlg1,
                                 std::size_t xDim,
                                 std::vector<NLOptWrAlgorithm> &res) const;

  /**
   * search for MLSL algorithms
   * @param p4pAlg1 search criteria
   * @param xDim number of paramaters (size of x)
   * @param hasConstraints constraint flag
   * @param res algorithms that match or noting
   * @return success of search
   */
  bool findSearchAlgorithmMlsl(const NLOptWrSearchAlgoParam &p4pAlg1,
                               std::size_t xDim, bool hasConstraints,
                               std::vector<NLOptWrAlgorithm> &res) const;

  /**
   * search for local and global algorithms
   * @param p4pAlg1 search criteria
   * @param xDim number of paramaters (size of x)
   * @param hasConstraints constraint flag
   * @param res algorithms that match or noting
   * @return success of search
   */
  bool findSearchAlgorithmLG(const NLOptWrSearchAlgoParam &p4pAlg1,
                             std::size_t xDim, bool hasConstraints,
                             std::vector<NLOptWrAlgorithm> &res) const;

  // ------------------------------------------------------------------------------------------------

  /* *
   * find prefered search Algorithm
   * @param p4pAlg1 1st search criterium
   * @param xDim dimension of search vector x
   * @param res result vector
   * @return success (true means OK)
   */
  // bool findSearchAlgorithm(const P4PrefAlgorithm& p4pAlg1, std::size_t xDim,
  // bool ignoreConstraints, bool searchLocalOrGlobal,
  // std::vector<NlOptAlgorithm>& res) const;

  // ------------------------------------------------------------------------------------------------

  /**
   * find default search Algorithm
   * @param p4pAlg search criterium
   * @param xDim dimension of search vector x
   * @param ignoreConstraints ignore constraints flag
   * @param searchLocalOrGlobal  search global or local flag
   * @param isNotMLSL is not MLSL flag
   * @param res result vector
   * @return success (true means OK)
   */
  bool findSearchAlgorithm(const NLOptWrSearchAlgoParam &p4pAlg,
                           std::size_t xDim, bool ignoreConstraints,
                           bool searchLocalOrGlobal, bool isNotMLSL,
                           std::vector<NLOptWrAlgorithm> &res) const;

  // ------------------------------------------------------------------------------------------------

  /**
   * test if algorithm is auglag
   * @param alg enum of algorithm
   * @return result (true means name="*AUGLAG*")
   */
  bool isAuglag(nlopt::algorithm alg) const;

  /**
   * test if algorithm at index(idx) is auglag
   * @param idx index
   * @return result (true means name="*AUGLAG*")
   */
  bool isAuglag(std::size_t idx) const;

  /**
   * test if algorithm is MSL
   * @param alg enum of algorithm
   * @return result (true means name="*AUGLAG*")
   */
  bool isMLSL(nlopt::algorithm alg) const;

  /**
   * test if algorithm at index(idx) is MSL
   * @param idx index
   * @return result (true means name="*AUGLAG*")
   */
  bool isMLSL(std::size_t idx) const;

  // ------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------

private:
  ///
  std::vector<NLOptWrAlgorithm> getAvailableAlgorithms(
      const NLOptWrSearchAlgoParam &p4pAlg,
      const std::vector<class NLOptWrAlgorithm> &availableAlgs,
      std::size_t xDim, bool ignoreConstraints, bool searchLocalOrGlobal,
      bool isNotMLSL) const;

  /// available algoritms
  static const std::vector<class NLOptWrAlgorithm> nlOptAlgorithms;

  /// map of search strategies
  std::map<nlopt::algorithm, size_t> sStrategy2Idx;

  /// preferred algoritms
  std::vector<NLOptWrAlgorithm> preferedOptAlgorithms;

  /// debug flag
  int d = 0;
};

} // namespace nloptwr

#endif // NLOPT_PARAMFACTORY_H
