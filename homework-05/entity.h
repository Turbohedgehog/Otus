#pragma once

#include <vector>

#include "common_types.h"

namespace homework_05 {

class Entity {
 public:
  Entity(EnityId id);
  EnityId GetId() const;

  void AddComponent(ComponentPtr component);
  const std::vector<ComponentPtr>& GetComponents() const;  

 private:
  EnityId id_;
  std::vector<ComponentPtr> components_;
};

}  // namespace homework_05
