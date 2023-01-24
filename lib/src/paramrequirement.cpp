
#include "nloptwr/paramrequirement.h"

namespace nloptwr {

ParamRequirement::ParamRequirement(bool isRequirement) {}

ParamRequirement::~ParamRequirement() {}

bool ParamRequirement::isFullfilled(bool property) {
  return (requiredProperty) ? property : true;
}

void ParamRequirement::setReqired(bool isRequired) {
  requiredProperty = isRequired;
}

bool ParamRequirement::isRequiredProperty() { return requiredProperty; }

} // namespace nloptwr
