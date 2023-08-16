#include "bulk.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>

namespace homework_07 {

BulkParser::BulkParser(std::size_t block_size)
  : block_size_(block_size) {
  command_map_ = {
    {"{", [this] {
      ++block_counter_;
      if (block_counter_ == 1) {
        ExecuteAndFlushCommands();
      }
    }},
    {"}", [this] {
      --block_counter_;
      if (block_counter_ == 0) {
        ExecuteAndFlushCommands();
      }
      block_counter_ = std::max(0, block_counter_);
    }},
    {"EOF", [this] {
      if (block_counter_ == 0) {
        ExecuteAndFlushCommands();
      }

      eof_ = true;
    }},
  };
}

void BulkParser::ExecuteAndFlushCommands() {
  if (commands_.empty()) {
    return;
  }

  for (auto& executor : bulk_executors_) {
    executor->AcceptCommand(*this);
  }

  commands_.clear();
}

void BulkParser::Run() {
  std::string cur_command;
  while(!eof_) {
    std::cin >> cur_command;
    auto command_it = command_map_.find(cur_command);
    if (command_it == command_map_.end()) {
      if (commands_.empty()) {
        command_timestamp_ = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
      }

      commands_.push_back(cur_command);
      if (commands_.size() >= block_size_ && block_counter_ == 0) {
        ExecuteAndFlushCommands();
      }
    } else {
      command_it->second();
    }
  }
}

void BulkParser::AppendBulkExecutor(BulkExecutorPtr bulk_executor) {
  bulk_executors_.emplace_back(bulk_executor);
}

unsigned long long BulkParser::GetCommandTimestamp() const {
  return command_timestamp_;
}

const std::vector<std::string>& BulkParser::GetCommands() const {
  return commands_;
}



void BulkPrinter::AcceptCommand(const BulkParser& bulk_parser) {
  std::cout << "bulk: ";
  const auto& commands = bulk_parser.GetCommands();
  std::cout << *commands.begin();
  std::for_each(std::next(commands.begin()), commands.end(), [] (auto cmd) {
    std::cout << ", " << cmd;
  });

  std::cout << '\n';
}

void BulkFileWriter::AcceptCommand(const BulkParser& bulk_parser) {
  std::ofstream file_stream;
  auto timestamp = bulk_parser.GetCommandTimestamp();
  const auto& commands = bulk_parser.GetCommands();
  std::string filename = std::string("bulk") + std::to_string(timestamp) + std::string(".log");
  file_stream.open(filename);
  file_stream << *commands.begin();
  std::for_each(std::next(commands.begin()), commands.end(), [&] (auto cmd) {
    file_stream << ", ";
    file_stream << cmd;
  });

  file_stream.close();
}


}  // namespace homework_07
