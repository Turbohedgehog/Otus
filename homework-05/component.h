#pragma once

#include <cassert>
#include <string>

#include "common_types.h"

namespace homework_05 {

class ComponentBase {
 public:
  virtual ~ComponentBase() = 0;
  virtual const std::string_view& GetComponentName() const = 0;
  virtual ComponentHandlerPtr CreateHandler(ComponentHandlerFactoryWeakPtr factory) const = 0;

  template <typename T>
  static std::shared_ptr<T> CastComponent(ComponentWeakPtr component) {
    std::shared_ptr<T> casted_component = std::static_pointer_cast<T>(component.lock());
    assert(casted_component);

    return casted_component;
  }
};

class NameComponent : public ComponentBase {
 public:
  const std::string_view& GetComponentName() const override;
  ComponentHandlerPtr CreateHandler(ComponentHandlerFactoryWeakPtr factory) const override;
  void SetName(const std::string& new_name);
  const std::string& GetName() const;

 public:
  static constexpr std::string_view kComponentName = "NameComponent";

 private:
  std::string name_;
};

class Coordinate2DComponent : public ComponentBase {
 public:
  const std::string_view& GetComponentName() const override;
  ComponentHandlerPtr CreateHandler(ComponentHandlerFactoryWeakPtr factory) const override;
  float GetX() const;
  float GetY() const;
  void SetX(float x);
  void SetY(float y);

 public:
  static constexpr std::string_view kComponentName = "Coordinate2DComponent";

 private:
  float x_ = 0.f;
  float y_ = 0.f;
};

class ShapeComponent : public ComponentBase {};

class CircleComponent : public ShapeComponent {
 public:
  const std::string_view& GetComponentName() const override;
  ComponentHandlerPtr CreateHandler(ComponentHandlerFactoryWeakPtr factory) const override;
  float GetRadius() const;
  void SetRadius(float radius);

 public:
  static constexpr std::string_view kComponentName = "CircleComponent";

 private:
  float radius_ = 0.f;
};

class RectangleComponent : public ShapeComponent {
 public:
  const std::string_view& GetComponentName() const override;
  ComponentHandlerPtr CreateHandler(ComponentHandlerFactoryWeakPtr factory) const override;
  float GetWidth() const;
  float GetHeight() const;
  void SetWidth(float width);
  void SetHeight(float height);

 public:
  static constexpr std::string_view kComponentName = "RectangleComponent";

 private:
  float width_ = 0.f;
  float height_ = 0.f;
};

}  // namespace homework_05
