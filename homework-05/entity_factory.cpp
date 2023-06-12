#include "entity_factory.h"

#include "entity.h"
#include "component.h"

namespace homework_05 {

EntityPtr EntityFactory::CreateDefaultShape(unsigned int id) {
  auto default_shape = std::make_shared<Entity>(id);
  auto coordinate_component = std::make_shared<Coordinate2DComponent>();
  default_shape->AddComponent(coordinate_component);

  return default_shape;
}

EntityPtr EntityFactory::CreateDefaultCircle(unsigned int id) {
  auto default_circle = EntityFactory::CreateDefaultShape(id);
  default_circle->AddComponent(std::make_shared<CircleComponent>());

  return default_circle;
}

EntityPtr EntityFactory::CreateDefaultRectangle(unsigned int id) {
  auto default_rectangle = EntityFactory::CreateDefaultShape(id);
  default_rectangle->AddComponent(std::make_shared<RectangleComponent>());

  return default_rectangle;
}

void EntityFactory::SetEntityName(EntityPtr entity, const std::string& name) {
  std::shared_ptr<NameComponent> name_component;
  const auto& components = entity->GetComponents();
  for (auto& component : components) {
    if (component->GetComponentName() == NameComponent::kComponentName) {
      name_component = std::static_pointer_cast<NameComponent>(component);
      break;
    }
  }

  if (!name_component) {
    name_component = std::make_shared<NameComponent>();
    entity->AddComponent(name_component);
  }

  name_component->SetName(name);
}

}  // namespace homework_05
