#pragma once

#include <memory>

#include "common_types.h"

namespace homework_05 {

class ComponentHandlerFactory {
 public:
  virtual ComponentHandlerPtr CreateNameComponentHandler() const = 0;
  virtual ComponentHandlerPtr CreateCoordinate2DComponentHandler() const = 0;
  virtual ComponentHandlerPtr CreateCircleComponentHandler() const = 0;
  virtual ComponentHandlerPtr CreateRectangleComponentHandler() const = 0;
};

class ComponentHandler : public std::enable_shared_from_this<ComponentHandler> {
 public:
  virtual void DrawProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const = 0;
  virtual void SetProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const = 0;
};

}  // namespace homework_05
