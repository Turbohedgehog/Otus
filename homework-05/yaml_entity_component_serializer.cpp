#include "yaml_entity_component_serializer.h"

#include <fstream>
#include <stdexcept>

#include "yaml-cpp/yaml.h"

#include "entity.h"
#include "component.h"

namespace homework_05 {

class YamlComponentSerializer {
 public:
  virtual YAML::Node Serialize(ComponentPtr component) const {
    YAML::Node component_node;
    component_node["type"] = component->GetComponentName().data();

    return component_node;
  }

  virtual ComponentPtr ParseComponent(const YAML::Node& node) const = 0;

  static std::string GetType(const YAML::Node& node) {
    auto type_node = node["type"];
    if (!type_node) {
      return std::string();
    }

    return type_node.as<std::string>();
  }
};

class NameComponentSerializer : public YamlComponentSerializer {
 public:
  YAML::Node Serialize(ComponentPtr component) const {
    auto node = YamlComponentSerializer::Serialize(component);
    auto name_component = ComponentBase::CastComponent<NameComponent>(component);
    node["name"] = name_component->GetName();

    return node;
  }

  ComponentPtr ParseComponent(const YAML::Node& node) const {
    auto name_node = node["name"];
    if (!name_node) {
      throw std::invalid_argument("No name parameter in NameComponent");
    }

    auto name_component = std::make_shared<NameComponent>();
    name_component->SetName(name_node.as<std::string>());

    return name_component;
  }
};

class Coordinate2DComponentSerializer : public YamlComponentSerializer {
 public:
  YAML::Node Serialize(ComponentPtr component) const {
    auto node = YamlComponentSerializer::Serialize(component);
    auto coordinate_component = ComponentBase::CastComponent<Coordinate2DComponent>(component);
    node["x"] = coordinate_component->GetX();
    node["y"] = coordinate_component->GetY();

    return node;
  }

  ComponentPtr ParseComponent(const YAML::Node& node) const override {
    auto coordinate_x = node["x"];
    if (!coordinate_x) {
      throw std::invalid_argument("No x parameter in Coordinate2DComponent");
    }

    auto coordinate_y = node["y"];
    if (!coordinate_y) {
      throw std::invalid_argument("No y parameter in Coordinate2DComponent");
    }

    auto coordinate_component = std::make_shared<Coordinate2DComponent>();
    coordinate_component->SetX(coordinate_x.as<float>());
    coordinate_component->SetY(coordinate_y.as<float>());

    return coordinate_component;
  }
};

class CircleComponentSerializer : public YamlComponentSerializer {
 public:
  YAML::Node Serialize(ComponentPtr component) const override {
    auto node = YamlComponentSerializer::Serialize(component);
    auto circle_component = ComponentBase::CastComponent<CircleComponent>(component);
    node["radius"] = circle_component->GetRadius();

    return node;
  }

  ComponentPtr ParseComponent(const YAML::Node& node) const override {
    auto radius_node = node["radius"];
    if (!radius_node) {
      throw std::invalid_argument("No radius parameter in CircleComponent");
    }

    auto circle_component = std::make_shared<CircleComponent>();
    circle_component->SetRadius(radius_node.as<float>());

    return circle_component;
  }
};

class RectangleComponentSerializer : public YamlComponentSerializer {
 public:
  YAML::Node Serialize(ComponentPtr component) const {
    auto node = YamlComponentSerializer::Serialize(component);
    auto rectangle_component = ComponentBase::CastComponent<RectangleComponent>(component);
    node["width"] = rectangle_component->GetWidth();
    node["height"] = rectangle_component->GetHeight();

    return node;
  }

  ComponentPtr ParseComponent(const YAML::Node& node) const override {
    auto width_node = node["width"];
    if (!width_node) {
      throw std::invalid_argument("No width parameter in RectangleComponent");
    }

    auto height_node = node["height"];
    if (!height_node) {
      throw std::invalid_argument("No height parameter in RectangleComponent");
    }

    auto rectangle_component = std::make_shared<RectangleComponent>();
    rectangle_component->SetWidth(width_node.as<float>());
    rectangle_component->SetHeight(height_node.as<float>());

    return rectangle_component;
  }
};

YamlEntityComponentSerializer::YamlEntityComponentSerializer() {
  FillSerializers();
}

bool YamlEntityComponentSerializer::SaveEntities(
    const std::string& filename,
    const EntityContainer& entities) {
  std::ofstream fout(filename.c_str());

  YAML::Node doc;
  for ([[maybe_unused]] const auto& [id, entity] : entities) {
    YAML::Node entity_node;
    entity_node["id"] = entity->GetId();
    YAML::Node components_node;

    for (const auto& component_ptr : entity->GetComponents()) {
      auto it = component_serializers_.find(component_ptr->GetComponentName());
      if (it == component_serializers_.end()) {
        throw std::invalid_argument(
            std::string("Cannot find serializer for component '") +
            std::string(component_ptr->GetComponentName()) +
            std::string("'"));
        return false;
      }
      components_node.push_back(it->second->Serialize(component_ptr));
    }
    entity_node["components"] = components_node;
    
    doc.push_back(entity_node);
  }

  fout << doc;

  return true;
}

EntityContainer YamlEntityComponentSerializer::LoadEntities(const std::string& filename) {
  YAML::Node doc = YAML::LoadFile(filename);
  if (!doc.IsSequence()) {
    throw std::invalid_argument("Wrong document format");
  }

  EntityContainer entities;

  for (auto entity_node : doc) {
    auto id = entity_node["id"].as<unsigned int>();

    if (entities.find(id) != entities.end()) {
      throw std::invalid_argument(
        std::string("Dublicate entity id: ") +
        std::to_string(id)
      );
    }

    auto entity = std::make_shared<Entity>(id);
    auto components_node = entity_node["components"];
    if (components_node) {
      for (auto component_node : components_node) {
        std::string component_type = YamlComponentSerializer::GetType(component_node);
        std::string_view type_name_sv = component_type;
        auto parser_it = component_serializers_.find(type_name_sv);
        if (parser_it == component_serializers_.end()) {
            throw std::invalid_argument(
            std::string("Cannot find deserializer for component '") +
            component_type +
            "'");
        }

        auto component = parser_it->second->ParseComponent(component_node);
        if (!component) {
            throw std::invalid_argument(
            std::string("Cannot parse '") +
            component_type +
            std::string("' component"));
        }
        entity->AddComponent(component);
      }
    }

    entities[entity->GetId()] = entity;
  }

  return entities;
}

void YamlEntityComponentSerializer::FillSerializers() {
  component_serializers_[NameComponent::kComponentName] = std::make_shared<NameComponentSerializer>();
  component_serializers_[Coordinate2DComponent::kComponentName] = std::make_shared<Coordinate2DComponentSerializer>();
  component_serializers_[CircleComponent::kComponentName] = std::make_shared<CircleComponentSerializer>();
  component_serializers_[RectangleComponent::kComponentName] = std::make_shared<RectangleComponentSerializer>();
}

}  // namespace homework_05