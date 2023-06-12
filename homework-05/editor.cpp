#include "editor.h"

#include <stdexcept>

#include "toolset.h"
#include "entity_factory.h"
#include "yaml_entity_component_serializer.h"

namespace homework_05 {

Editor::Editor() {
  serializers_[kYamlSerializerDescription] = std::make_shared<YamlEntityComponentSerializer>();
}

bool Editor::Interact(ToolsetPtr toolset) {
  toolset->Draw(shared_from_this());

  return !GetExit();
}

bool Editor::GetExit() const {
  return exit_;
}

void Editor::SetExit(bool exit) {
  exit_ = exit;
}

void Editor::NewDocument() {
  entities_.clear();
}

bool Editor::ImportDocument(const std::string_view& format, const std::string& document_filename) {
  auto serialiser_it = serializers_.find(format);
  if (serialiser_it == serializers_.end()) {
    throw std::invalid_argument(std::string("Cannot find '") + std::string(format) + std::string("' importer"));

    return false;
  }

  entities_ = serialiser_it->second->LoadEntities(document_filename);
  new_entity_id_ = entities_.empty() ? 0 : entities_.rbegin()->first + 1;
  
  return true;
}

bool Editor::ExportDocument(const std::string_view& format, const std::string& document_filename) {

  auto serialiser_it = serializers_.find(format);
  if (serialiser_it == serializers_.end()) {
    throw std::invalid_argument(std::string("Cannot find '") + std::string(format) + std::string("' exporter"));

    return false;
  }

  return serialiser_it->second->SaveEntities(document_filename, GetEntities());
}

const EntityContainer& Editor::GetEntities() const {
  return entities_;
}

void Editor::AddEntityToContainer(EntityPtr entity) {
  entities_[new_entity_id_] = entity;
  ++new_entity_id_;
}

EntityWeakPtr Editor::AppendCircle() {
  auto cirlce_entity = EntityFactory::CreateDefaultCircle(new_entity_id_);
  //EntityFactory::SetEntityName(cirlce_entity, "Circle " + std::to_string(new_entity_id_));
  AddEntityToContainer(cirlce_entity);

  return cirlce_entity;
}

EntityWeakPtr Editor::AppendRectangle() {
  auto cirlce_entity = EntityFactory::CreateDefaultRectangle(new_entity_id_);
  //EntityFactory::SetEntityName(cirlce_entity, "Rectangle " + std::to_string(new_entity_id_));
  AddEntityToContainer(cirlce_entity);
  
  return cirlce_entity;
}

bool Editor::RemoveEnity(EnityId id) {
  auto it = entities_.find(id);
  if (it == entities_.end()) {
    return false;
  }

  entities_.erase(it);
  
  return true;
}

}  // namespace homework_05
