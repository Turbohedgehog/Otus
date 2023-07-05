#include "entity.h"

namespace homework_05 {

Entity::Entity(EnityId id)
  : id_(id) {}

EnityId Entity::GetId() const {
  return id_;
}

void Entity::AddComponent(ComponentPtr component) {
  components_.push_back(component);
}

const std::vector<ComponentPtr>& Entity::GetComponents() const {
  return components_;
}

}  // namespace homework_05