#pragma once

#include <iostream>

#include "toolset.h"

namespace homework_05 {

class ConsoleAction;
using ConsoleActionPtr = std::shared_ptr<ConsoleAction>;

class ConsoleToolset : public Toolset {
 public:
  ConsoleToolset();
  void Draw(const EditorWeakPtr& editor) override;
  void ImportDocument(const EditorWeakPtr& editor) override;
  void ExportDocument(const EditorWeakPtr& editor) override;
  void DrawShapes(const EditorWeakPtr& editor) override;
  void EditShape(EntityWeakPtr entity) override;

  static constexpr std::string_view kSingleSeparator = "---------------------------------------";
  static constexpr std::string_view kDoubleSeparator = "=======================================";

  template <typename... Ts>
  void Request(const std::string& request_text, Ts&... values) {
    std::cout << request_text;
    ((std::cin >> (values)), ...);
  }

  template <typename... Ts>
  void Print(Ts&&... values) {
    ((std::cout << std::forward<Ts>(values)), ...);
    std::cout << "\n";
  }

 protected:
  void DrawShape(EntityWeakPtr entity) override;

 private:
  void CreateActions();
  void RequestAction(const struct ActionContext& context);

  ConsoleActionPtr root_action_;
};

}  // namespace homework_05
