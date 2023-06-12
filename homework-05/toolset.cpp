#include "toolset.h"

#include "editor.h"
#include "component.h"

namespace homework_05 {

ComponentHandlerWeakPtr Toolset::GetComponentHandler(ComponentWeakPtr component) const {
  auto component_ptr = component.lock();
  auto it = component_handlers_.find(component_ptr->GetComponentName());
  if (it != component_handlers_.end()) {
    return it->second;
  }

  auto handler = component_ptr->CreateHandler(component_handler_factory_);
  component_handlers_[component_ptr->GetComponentName()] = handler;

  return handler;
}

void Toolset::DrawShapes(const EditorWeakPtr& editor) {
  const auto& entities = editor.lock()->GetEntities();
  for ([[maybe_unused]] const auto& [id, entity] : entities) {
    DrawShape(entity);
  }
}

}  // namespace homework_05
