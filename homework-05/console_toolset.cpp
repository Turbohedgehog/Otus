#include "console_toolset.h"

#include <functional>
#include <memory>
#include <string>

#include "entity.h"
#include "editor.h"
#include "console_component_handler.h"

namespace homework_05 {

using ConsoleActionWeakPtr = std::weak_ptr<ConsoleAction>;
using ActionCallback = std::function<void(const ActionContext&)>;

struct ActionContext {
  EditorWeakPtr editor;
  ToolsetWeakPtr toolset;
};

class ConsoleAction : std::enable_shared_from_this<ConsoleAction> {
 public:
  ConsoleAction(const std::string& title)
    : title_(title) {
  }

  void AppendChild(ConsoleActionPtr child) {
    children_.push_back(child);
  }

  void SetCallback(ActionCallback callback) {
    callback_ = callback;
  }

  bool Request(const ActionContext& context) {
    std::cout << ConsoleToolset::kDoubleSeparator << "\n";
    std::cout << title_ << '\n';
    if (!children_.empty()) {
      while (true) {
        std::cout << ConsoleToolset::kDoubleSeparator << "\n";
        for (std::size_t i = 0; i < children_.size(); ++i) {
          std::cout << (i + 1) << ". " << children_[i]->title_ << '\n';
        }
        std::cout << ConsoleToolset::kSingleSeparator << "\n";
        std::cout << "Input action number: ";
        std::string str_action_id;
        std::cin >> str_action_id;
        std::size_t action_id;
        try {
          action_id = std::stoi(str_action_id);
        } catch (...) {
          std::cout << "Wrong number id!\n";
          continue;
        }

        if (action_id == 0 && action_id > children_.size()) {
          std::cout << "Number id not in range!\n";
          continue;
        }

        if (children_[action_id - 1]->Request(context)) {
          break;
        }
      }
    }

    if (callback_) {
      callback_(context);
    }

    return true;
  }

 private:
  ActionCallback callback_;
  std::string title_;
  std::vector<ConsoleActionPtr> children_;
};

ConsoleToolset::ConsoleToolset() {
  component_handler_factory_ = std::make_shared<ConsoleComponentHandlerFactory>();
  CreateActions();
}

void ConsoleToolset::CreateActions() {
  root_action_ = std::make_shared<ConsoleAction>("Main menu");
  auto new_document = std::make_shared<ConsoleAction>("New document");
  auto import_document = std::make_shared<ConsoleAction>("Import document");
  auto export_document = std::make_shared<ConsoleAction>("Export document");
  auto draw_all = std::make_shared<ConsoleAction>("Draw all");
  auto add_shape = std::make_shared<ConsoleAction>("Add shape");
  auto remove_shape = std::make_shared<ConsoleAction>("Remove shape");
  auto quit_action = std::make_shared<ConsoleAction>("Quit");

  root_action_->AppendChild(new_document);
  root_action_->AppendChild(import_document);
  root_action_->AppendChild(export_document);
  root_action_->AppendChild(draw_all);
  root_action_->AppendChild(add_shape);
  root_action_->AppendChild(remove_shape);
  root_action_->AppendChild(quit_action);

  quit_action->SetCallback([](const ActionContext& context) {
    context.editor.lock()->SetExit(true);
  });

  auto new_doc_yes = std::make_shared<ConsoleAction>("Yes");
  auto action_no = std::make_shared<ConsoleAction>("No");
  new_document->AppendChild(new_doc_yes);
  new_document->AppendChild(action_no);
  new_doc_yes->SetCallback([](const ActionContext& context) {
    context.editor.lock()->NewDocument();

    std::cout << "New document created!\n";
  });

  auto import_doc_yes = std::make_shared<ConsoleAction>("Confirm import");
  import_doc_yes->SetCallback([](const ActionContext& context) {
    context.toolset.lock()->ImportDocument(context.editor);
  });
  import_document->AppendChild(import_doc_yes);
  import_document->AppendChild(action_no);

  auto export_doc_yes = std::make_shared<ConsoleAction>("Confirm export");
  export_doc_yes->SetCallback([](const ActionContext& context) {
    context.toolset.lock()->ExportDocument(context.editor);
  });
  export_document->AppendChild(export_doc_yes);
  export_document->AppendChild(action_no);

  draw_all->SetCallback([](const ActionContext& context) {
    context.toolset.lock()->DrawShapes(context.editor);
  });

  //add_shape
  auto add_circle = std::make_shared<ConsoleAction>("Add circle");
  add_circle->SetCallback([](const ActionContext& context) {
    auto circle = context.editor.lock()->AppendCircle();
    context.toolset.lock()->EditShape(circle);
  });

  auto add_rectangle = std::make_shared<ConsoleAction>("Add rectangle");
  add_rectangle->SetCallback([](const ActionContext& context) {
    auto rectangle = context.editor.lock()->AppendRectangle();
    context.toolset.lock()->EditShape(rectangle);
  });

  auto cancel = std::make_shared<ConsoleAction>("Cancel");

  add_shape->AppendChild(add_circle);
  add_shape->AppendChild(add_rectangle);
  add_shape->AppendChild(cancel);

  auto confirm_remove = std::make_shared<ConsoleAction>("Confirm remove shape");
  remove_shape->AppendChild(confirm_remove);
  remove_shape->AppendChild(cancel);

  confirm_remove->SetCallback([](const ActionContext& context) {
    std::cout << "Enter shape id: ";
    EnityId id;
    std::cin >> id;
    bool removed = context.editor.lock()->RemoveEnity(id);
    if (removed) {
      std::cout << "Enity [" << id << "] has been removed!\n";
    } else {
      std::cout << "Cannot remove entity " << id << "\n";
    }
  });
}

void ConsoleToolset::RequestAction(const ActionContext& context) {
  root_action_->Request(context);
}

void ConsoleToolset::ImportDocument(const EditorWeakPtr& editor) {
  std::string import_filename;
  std::cout << "Enter document filename: ";
  std::cin >> import_filename;
  try {
    bool res = editor.lock()->ImportDocument(kYamlSerializerDescription, import_filename);
    if (res) {
      std::cout << "Documment '" << import_filename << "' has been imported!\n";
    } else {
      std::cout << "Cannot import '" << import_filename << "'!\n";
    }
  } catch (std::exception& ex) {
    std::cout << "Cannot import '" << import_filename << "' by reason:\n";
    std::cout << ex.what() << "\n";
  }
}

void ConsoleToolset::ExportDocument(const EditorWeakPtr& editor) {
  std::string export_filename;
  std::cout << "Enter document filename: ";
  std::cin >> export_filename;
  try {
    bool res = editor.lock()->ExportDocument(kYamlSerializerDescription, export_filename);
    if (res) {
      std::cout << "Documment '" << export_filename << "' has been exported!\n";
    } else {
      std::cout << "Cannot export '" << export_filename << "'!\n";
    }
  } catch (std::exception& ex) {
    std::cout << "Cannot export '" << export_filename << "' by reason:\n";
    std::cout << ex.what() << "\n";
  }
}

void ConsoleToolset::Draw(const EditorWeakPtr& editor) {
  ActionContext context;
  context.editor = editor;
  context.toolset = shared_from_this();
  RequestAction(context);
}

void ConsoleToolset::DrawShapes(const EditorWeakPtr& editor) {
  std::cout << ConsoleToolset::kSingleSeparator << "\n";
  std::cout << "Shape list:\n";

  Toolset::DrawShapes(editor);
}

void ConsoleToolset::DrawShape(EntityWeakPtr entity) {
  std::cout << ConsoleToolset::kSingleSeparator << "\n";
  auto entity_ptr = entity.lock();
  std::cout << "Shape id: " << entity_ptr->GetId() << "\n";
  const auto& compoenents = entity_ptr->GetComponents();
  auto this_ptr = shared_from_this();
  for (const auto& component : compoenents) {
    auto handler = GetComponentHandler(component);
    handler.lock()->DrawProperties(component, this_ptr);
  }
  std::cout << ConsoleToolset::kSingleSeparator << "\n";
}

void ConsoleToolset::EditShape(EntityWeakPtr entity) {
  auto entity_ptr = entity.lock();
  const auto& compoenents = entity_ptr->GetComponents();
  auto this_ptr = shared_from_this();
  for (const auto& component : compoenents) {
    auto handler = GetComponentHandler(component);
    handler.lock()->SetProperties(component, this_ptr);
  }
  std::cout << ConsoleToolset::kSingleSeparator << "\n";
}

}  // namespace homework_05
