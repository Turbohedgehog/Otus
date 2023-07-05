#include <iostream>

#include "console_toolset.h"
#include "editor.h"

int main (int, char **) {
  auto console_toolset = std::make_shared<homework_05::ConsoleToolset>();
  auto editor = std::make_shared<homework_05::Editor>();

  while (editor->Interact(console_toolset)) { }

  return 0;
}