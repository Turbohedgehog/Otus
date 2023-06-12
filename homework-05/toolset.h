#pragma once

#include <string>
#include <unordered_map>

#include "common_types.h"

namespace homework_05 {

class Toolset : public std::enable_shared_from_this<Toolset> {
 public:
  virtual void Draw(const EditorWeakPtr& editor) = 0;
  virtual void ImportDocument(const EditorWeakPtr& editor) = 0;
  virtual void ExportDocument(const EditorWeakPtr& editor) = 0;
  virtual void EditShape(EntityWeakPtr entity) = 0;
  virtual void DrawShapes(const EditorWeakPtr& editor);

 protected:
  virtual void DrawShape(EntityWeakPtr entity) = 0;

  ComponentHandlerWeakPtr GetComponentHandler(ComponentWeakPtr component) const;

  ComponentHandlerFactoryPtr component_handler_factory_;

 private:
  mutable std::unordered_map<std::string_view, ComponentHandlerPtr> component_handlers_;  
};

}  // namespace homework_05
