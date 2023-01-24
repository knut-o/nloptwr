
#ifndef NLOPT_PARAMFACTORY_H
#define NLOPT_PARAMFACTORY_H

#include "nlopt.hpp"
#include "nloptwralgorithm.h"
#include "nloptwrsearchalgoparam.h"
#include "nloptwrsearchalgoparamd.h"

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
  /// internally used enum to search algorithms
  enum A_ENUM {
    A_AUGLAG_EQ,
    A_AUGLAG_EQ_SUB,
    A_AUGLAG_NE,
    A_AUGLAG_NE_SUB,
    A_MLSL,
    A_MLSL_SUB,
    A_L,
    A_G,
    A_R
  };

public:
  /// constructor
  NLOptWrParamFactory();

  /// destructor
  ~NLOptWrParamFactory();

  // ============================================================================

  /**
   * get a set of agorithms
   * @param p4pAlg search parameter (except dimension of parameter x)
   * @return algorithms
   */
  std::vector<NLOptWrAlgorithm>
  getAlgorithm(const NLOptWrSearchAlgoParamD &p4pAlg) const;

  /**
   * get a set of agorithms
   * @param searchStrategy search strategy enum
   * @param hasEqConstraints equality constraints constraints flag
   * @param hasNeConstraints nonequality constraints constraints flag
   * @param useGradient use gradient
   * @param xDim dimension of parameter x
   */
  std::vector<NLOptWrAlgorithm>
  getAlgorithm(SSTRAT searchStrategy, bool hasEqConstraints,
               bool hasNeConstraints, bool useGradient, std::size_t xDim) const;

  /**
   * get a set of agorithms
   * @param p4pAlg search parameter (except dimension of parameter x)
   * @param xDim dimension of parameter x
   */
  std::vector<NLOptWrAlgorithm>
  getAlgorithm(const NLOptWrSearchAlgoParam &p4pAlg, std::size_t xDim) const;

  // ============================================================================

  /**
   * get name of algorithm
   * @param alg enum of algorithm
   * @return name of algorithm
   */
  const std::string &getNameOfAlgorithm(nlopt::algorithm alg) const;

  // ============================================================================

  /**
   * ask for preferred algorithm
   * @param p4pAlg search parameter
   * @return flag (true if a preferred algorithm exists)
   */
  bool hasPreferedAlgorithms() const;

  /**
   * delete a preferred algorithm
   * @param p4pAlg search parameter
   * @return flag (true if a preferred algorithm is deleted)
   */

  /**
   * delete all preferred algorithms
   */
  void deletePreferedAlgorithms();

  /**
   * set a preferred algorithm
   * @param algx enum of algorithm
   * @param xDimMin minimal number of parameters (-1 means no change)
   * @param xDimMax maximal number of parameters (-1 means no change)
   * @return flag (true if a preferred algorithm is deleted)
   */
  void setPreferedAlgorithm(nlopt::algorithm algx, int xDimMin = -1,
                            int xDimMax = -1);

  /**
   * set a preferred algorithm
   * @param algxs enum of algorithms
   * @return flag (true if a preferred algorithm is deleted)
   */
  void setPreferedAlgorithms(const std::vector<nlopt::algorithm> &algxs);

  /**
   * get preferred algorithms
   * @return flag (true if a preferred algorithm is deleted)
   */
  const std::vector<NLOptWrAlgorithm> &getPreferedAlgorithms() const;

  // ============================================================================

  /**
   * search the available algorithms
   * from the prefered algorithms pool
   * and then from the ordinary list of algorithms
   * @param algE enum ti distinguish algorithms
   * @param p4pAlg search criteria object
   * @param xDim dimension (number of free parameters)
   * @return list (vector) of algorithms
   */
  std::vector<NLOptWrAlgorithm>
  getAvailableAlgorithms(A_ENUM algE,
                         const NLOptWrSearchAlgoParamD &p4pAlg) const;

  /**
   * internally used search algorithm
   * @param algE enum ti distinguish algorithms
   * @param p4pAlg search criteria object
   * @param availableAlgs list to be searched
   * @param xDim dimension (number of free parameters)
   * @return list (vector) of algorithms
   */
  std::vector<NLOptWrAlgorithm> getAvailableAlgorithms(
      A_ENUM algE, const NLOptWrSearchAlgoParamD &p4pAlg,
      const std::vector<class NLOptWrAlgorithm> &availableAlgs) const;

  /**
   * get the result from the the lists of algorithms
   * @param algs1 list of first algoritms
   * @param algs2 list of subalgoritms
   * @param hasSubalg flag if subalgorithms are required
   * @return list of valid algorithms or an emty list
   */
  static std::vector<NLOptWrAlgorithm>
  getResultAlgorithms(const std::vector<NLOptWrAlgorithm> &algs1,
                      const std::vector<NLOptWrAlgorithm> &algs2,
                      bool hasSubAlg);

  // ============================================================================

protected:
  /// get name of algorithm of index x
  const std::string &getNameOfIdx(size_t idx) const;

  /// get index of algorithm with enum of algorithm
  size_t getIdxOfEnum(nlopt::algorithm) const;

  /// get enum of algorithm with index
  nlopt::algorithm getEnumOfIdx(size_t) const;

  // ============================================================================

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

  // ============================================================================

private:
  /// available algorithms
  static const std::vector<class NLOptWrAlgorithm> nlOptAlgorithms;

  /// map of search strategies
  std::map<nlopt::algorithm, size_t> sStrategy2Idx;

  /// preferred algorithms
  std::vector<NLOptWrAlgorithm> preferedOptAlgorithms;

  /// debug flag
  int d = 0;
};

} // namespace nloptwr

#endif // NLOPT_PARAMFACTORY_H
