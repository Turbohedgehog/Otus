#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "entity_component_serializer.h"

namespace homework_05 {

class YamlComponentSerializer;

using YamlComponentSerializerPtr = std::shared_ptr<YamlComponentSerializer>;

class YamlEntityComponentSerializer : public EntityComponentSerializer {
 public:
  YamlEntityComponentSerializer();

  bool SaveEntities(const std::string& filename, const EntityContainer& entities) override;
  EntityContainer LoadEntities(const std::string& filename) override;

 private:
  void FillSerializers();

 private:
  std::unordered_map<std::string_view, YamlComponentSerializerPtr> component_serializers_;
};

}  // namespace homework_05
