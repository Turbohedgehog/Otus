#pragma once

#include <string>
#include <vector>

#include "common_types.h"

namespace homework_05 {

class EntityComponentSerializer {
 public:
  virtual bool SaveEntities(const std::string& filename, const EntityContainer& entities) = 0;
  virtual EntityContainer LoadEntities(const std::string& filename) = 0;
};

}  // namespace homework_05
