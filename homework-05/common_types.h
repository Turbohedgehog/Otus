#pragma once

#include <string>
#include <memory>
#include <map>

namespace homework_05 {

class Editor;
using EditorPtr = std::shared_ptr<Editor>;
using EditorWeakPtr = std::weak_ptr<Editor>;

class Toolset;
using ToolsetPtr = std::shared_ptr<Toolset>;
using ToolsetWeakPtr = std::weak_ptr<Toolset>;

class Entity;
using EnityId = unsigned int;
using EntityPtr = std::shared_ptr<Entity>;
using EntityWeakPtr = std::weak_ptr<Entity>;
using EntityContainer = std::map<EnityId, EntityPtr>;

class ComponentBase;
using ComponentPtr = std::shared_ptr<ComponentBase>;
using ComponentWeakPtr = std::weak_ptr<ComponentBase>;

class ComponentHandler;
using ComponentHandlerPtr = std::shared_ptr<ComponentHandler>;
using ComponentHandlerWeakPtr = std::weak_ptr<ComponentHandler>;

class ComponentHandlerFactory;
using ComponentHandlerFactoryPtr = std::shared_ptr<ComponentHandlerFactory>;
using ComponentHandlerFactoryWeakPtr = std::weak_ptr<ComponentHandlerFactory>;

class EntityComponentSerializer;
using EntityComponentSerializerPtr = std::shared_ptr<EntityComponentSerializer>;

static constexpr std::string_view kYamlSerializerDescription = "YAML";

}  // namespace homework_05
