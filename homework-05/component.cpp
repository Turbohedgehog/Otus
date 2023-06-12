#include "component.h"

#include "component_handler.h"

namespace homework_05 {

ComponentBase::~ComponentBase() {}

const std::string_view& NameComponent::GetComponentName() const {
  return NameComponent::kComponentName;
}

ComponentHandlerPtr NameComponent::CreateHandler(ComponentHandlerFactoryWeakPtr factory) const {
  return factory.lock()->CreateNameComponentHandler();
}

void NameComponent::SetName(const std::string& new_name) {
  name_ = new_name;
}

const std::string& NameComponent::GetName() const {
  return name_;
}

const std::string_view& Coordinate2DComponent::GetComponentName() const {
  return Coordinate2DComponent::kComponentName;
}

ComponentHandlerPtr Coordinate2DComponent::CreateHandler(ComponentHandlerFactoryWeakPtr factory) const {
  return factory.lock()->CreateCoordinate2DComponentHandler();
}

float Coordinate2DComponent::GetX() const {
  return x_;
}

float Coordinate2DComponent::GetY() const {
  return y_;
}

void Coordinate2DComponent::SetX(float x) {
  x_ = x;
}

void Coordinate2DComponent::SetY(float y) {
  y_ = y;
}

const std::string_view& CircleComponent::GetComponentName() const {
  return CircleComponent::kComponentName;
}

ComponentHandlerPtr CircleComponent::CreateHandler(ComponentHandlerFactoryWeakPtr factory) const {
  return factory.lock()->CreateCircleComponentHandler();
}

float CircleComponent::GetRadius() const {
  return radius_;
}

void CircleComponent::SetRadius(float radius) {
  radius_ = radius;
}

const std::string_view& RectangleComponent::GetComponentName() const {
  return RectangleComponent::kComponentName;
}

ComponentHandlerPtr RectangleComponent::CreateHandler(ComponentHandlerFactoryWeakPtr factory) const {
  return factory.lock()->CreateRectangleComponentHandler();
}

float RectangleComponent::GetWidth() const {
  return width_;
}

float RectangleComponent::GetHeight() const {
  return height_;
}

void RectangleComponent::SetWidth(float width) {
  width_ = width;
}

void RectangleComponent::SetHeight(float height) {
  height_ = height;
}

}  // namespace homework_05
