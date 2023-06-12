#pragma once

#include "component_handler.h"

namespace homework_05 {

class ConsoleComponentHandlerFactory : public ComponentHandlerFactory {
 public:
  ComponentHandlerPtr CreateNameComponentHandler() const override;
  ComponentHandlerPtr CreateCoordinate2DComponentHandler() const override;
  ComponentHandlerPtr CreateCircleComponentHandler() const override;
  ComponentHandlerPtr CreateRectangleComponentHandler() const override;
};

}  // namespace homework_05
