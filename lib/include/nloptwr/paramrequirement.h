
#ifndef PARAM_REQUIREMENT_H
#define PARAM_REQUIREMENT_H

namespace nloptwr {

/**
 * Class to compare requirements
 */
class ParamRequirement {

public:
  /**
   * constructor
   * @param  isRequirement flag (true means the a requirement exists, otherwise
   * false)
   */
  ParamRequirement(bool isRequirement = true);

  /// destructor
  ~ParamRequirement();

  /**
   * test if requirement is isFullfilled
   * @param property propery to be tested
   * @return true if requirement is fullfilled, otherwise false
   */
  bool isFullfilled(bool property);

  /**
   * set / unset als quiered property
   * @param isRequired equired flag (default=true)
   */
  void setReqired(bool isRequired = true);

  /**
   * get status
   * @return true if it is a requied property
   */
  bool isRequiredProperty();

private:
  /// requirement flag (true means the a requirement exists, otherwise false)
  bool requiredProperty;
};

} // namespace nloptwr

#endif // PARAM_REQUIREMENT_H
