#pragma once

#include <unordered_map>
#include <string>

#include "common_types.h"

namespace homework_05 {

class Editor : public std::enable_shared_from_this<Editor> {
 public:
  Editor();

  bool Interact(ToolsetPtr toolset);
  bool GetExit() const;
  void SetExit(bool exit); 
  void NewDocument();
  bool ImportDocument(const std::string_view& format, const std::string& document_filename);
  bool ExportDocument(const std::string_view& format, const std::string& document_filename);
  bool RemoveEnity(EnityId id);
  const EntityContainer& GetEntities() const;
  EntityWeakPtr AppendCircle();
  EntityWeakPtr AppendRectangle();

 private:
  void AddEntityToContainer(EntityPtr entity);

 private:
  bool exit_ = false;
  EntityContainer entities_;
  EnityId new_entity_id_ = 0;
  std::unordered_map<std::string_view, EntityComponentSerializerPtr> serializers_;
};

}  // namespace homework_05
