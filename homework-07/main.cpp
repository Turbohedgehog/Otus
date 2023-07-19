
#include <iostream>
#include <cstdlib>
#include <map>
#include <functional>
#include <vector>
#include <algorithm>

//#define TEST

#ifdef TEST
#include <sstream>
#endif // #ifdef TEST

int main (int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Command couter argument is not set. Start the program by '.\\bulk <N>' where <N> is command couter limit.";
    return 1;
  }

  const std::size_t block_size = static_cast<std::size_t>(std::atoi(argv[1]));

  #ifdef TEST
  //std::istringstream in("cmd1\ncmd2\ncmd3\ncmd4\ncmd5\nEOF");
  std::istringstream in("cmd1\ncmd2\n{\ncmd3\ncmd4\n}\n{\ncmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}\n{\ncmd10\ncmd11\nEOF");
  std::cin.rdbuf(in.rdbuf());
#endif

  bool eof = false;
  std::string cur_command;
  std::vector<std::string> commands;
  int block_counter = 0;

  auto print_and_clear_commands = [&] {
    if (commands.empty()) {
      return;
    }

    std::cout << "bulk: ";
    std::cout << *commands.begin();
    std::for_each(std::next(commands.begin()), commands.end(), [] (auto cmd) {
      std::cout << ", " << cmd;
    });

    std::cout << '\n';
    commands.clear();
  };

  std::map<std::string, std::function<void()>> command_map = {
    {"{", [&] {
      ++block_counter;
      if (block_counter == 1) {
        print_and_clear_commands();
      }
    }},
    {"}", [&] {
      --block_counter;
      if (block_counter == 0) {
        print_and_clear_commands();
      }
      block_counter = std::max(0, block_counter);
    }},
    {"EOF", [&] {
      if (block_counter == 0) {
        print_and_clear_commands();
      }

      eof = true;
    }},
  };

  while(!eof) {
    std::cin >> cur_command;
    auto command_it = command_map.find(cur_command);
    if (command_it == command_map.end()) {
      commands.push_back(cur_command);
      if (commands.size() >= block_size && block_counter == 0) {
        print_and_clear_commands();
      }
    } else {
      command_it->second();
    }
  }

  return 0;
}