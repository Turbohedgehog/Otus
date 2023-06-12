#pragma once

#include <string>

#include "common_types.h"

namespace homework_05 {

class EntityFactory {
 public:
  static EntityPtr CreateDefaultCircle(unsigned int id);
  static EntityPtr CreateDefaultRectangle(unsigned int id);
  static void SetEntityName(EntityPtr entity, const std::string& name);

 private:
  static EntityPtr CreateDefaultShape(unsigned int id);

};

}  // namespace homework_05
