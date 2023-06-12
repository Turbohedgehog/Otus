#include "console_component_handler.h"

#include <cassert>

#include "common_types.h"
#include "component.h"
#include "console_toolset.h"

namespace homework_05 {

using ConsoleToolsetPtr = std::shared_ptr<ConsoleToolset>;

class ConsoleComponentHelper {
 public:
  static ConsoleToolsetPtr CastConsoleToolset(ToolsetWeakPtr toolset){
    auto console_toolset = std::static_pointer_cast<ConsoleToolset>(toolset.lock());
    assert(console_toolset);

    return console_toolset;
  }

  template <typename T>
  static std::shared_ptr<T> CastComponent(ComponentWeakPtr component) {
    std::shared_ptr<T> casted_component = std::static_pointer_cast<T>(component.lock());
    assert(casted_component);

    return casted_component;
  }
};

class ConsoleNameComponentHandler : public ComponentHandler {
 public:
  void DrawProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const override {
    auto console_toolset = ConsoleComponentHelper::CastConsoleToolset(toolset);
    auto name_component = ComponentBase::CastComponent<NameComponent>(component);
    console_toolset->Print("Name: ", name_component->GetName());
  }

  void SetProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const override {
    auto console_toolset = ConsoleComponentHelper::CastConsoleToolset(toolset);
    auto name_component = ComponentBase::CastComponent<NameComponent>(component);
    std::string new_name;
    console_toolset->Request("Enter new name: ", new_name);
    name_component->SetName(new_name);
  }
};

class ConsoleCoordinate2DComponentHandler : public ComponentHandler {
 public:
  void DrawProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const override {
    auto console_toolset = ConsoleComponentHelper::CastConsoleToolset(toolset);
    auto coordinate_component = ComponentBase::CastComponent<Coordinate2DComponent>(component);
    console_toolset->Print("X: ", coordinate_component->GetX());
    console_toolset->Print("Y: ", coordinate_component->GetY());
  }

  void SetProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const override {
    auto console_toolset = ConsoleComponentHelper::CastConsoleToolset(toolset);
    auto coordinate_component = ComponentBase::CastComponent<Coordinate2DComponent>(component);
    float new_x, new_y;
    console_toolset->Request("Enter new X: ", new_x);
    console_toolset->Request("Enter new Y: ", new_y);
    coordinate_component->SetX(new_x);
    coordinate_component->SetY(new_y);
  }
};

class ConsoleCircleComponentHandler : public ComponentHandler {
 public:
  void DrawProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const override {
    auto console_toolset = ConsoleComponentHelper::CastConsoleToolset(toolset);
    auto circle_component = ComponentBase::CastComponent<CircleComponent>(component);
    console_toolset->Print("Circle Radius: ", circle_component->GetRadius());
  }

  void SetProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const override {
    auto console_toolset = ConsoleComponentHelper::CastConsoleToolset(toolset);
    auto circle_component = ComponentBase::CastComponent<CircleComponent>(component);
    float new_radius;
    console_toolset->Request("Enter new radius: ", new_radius);
    circle_component->SetRadius(new_radius);
  }
};

class ConsoleRectangleComponentHandler : public ComponentHandler {
 public:
  void DrawProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const override {
    auto console_toolset = ConsoleComponentHelper::CastConsoleToolset(toolset);
    auto rectangle_component = ComponentBase::CastComponent<RectangleComponent>(component);
    console_toolset->Print(
        "Rectangle size (width, height): ",
        rectangle_component->GetWidth(),
        " ",
        rectangle_component->GetHeight()
    );
  }

  void SetProperties(ComponentWeakPtr component, ToolsetWeakPtr toolset) const override {
    auto console_toolset = ConsoleComponentHelper::CastConsoleToolset(toolset);
    auto rectangle_component = ComponentBase::CastComponent<RectangleComponent>(component);
    float new_width, new_height;
    console_toolset->Request("Enter new width: ", new_width);
    console_toolset->Request("Enter new height: ", new_height);
    rectangle_component->SetWidth(new_width);
    rectangle_component->SetHeight(new_height);
  }
};

ComponentHandlerPtr ConsoleComponentHandlerFactory::CreateNameComponentHandler() const {
  return std::make_shared<ConsoleNameComponentHandler>();
}

ComponentHandlerPtr ConsoleComponentHandlerFactory::CreateCoordinate2DComponentHandler() const {
  return std::make_shared<ConsoleCoordinate2DComponentHandler>();
}

ComponentHandlerPtr ConsoleComponentHandlerFactory::CreateCircleComponentHandler() const {
  return std::make_shared<ConsoleCircleComponentHandler>();
}

ComponentHandlerPtr ConsoleComponentHandlerFactory::CreateRectangleComponentHandler() const {
  return std::make_shared<ConsoleRectangleComponentHandler>();
}

}  // namespace homework_05

