#include "bulk.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>

namespace homework_09 {

BulkContext::BulkContext(std::size_t block_size)
    : kBlockSize(block_size) {
}

ExecutorBase::ExecutorBase(
    ExecutorHandler& execution_handler)
    : execution_handler_(execution_handler) {
  executor_thread_ = std::thread([this] { Loop(); });
}

ExecutorBase::~ExecutorBase() {}

void ExecutorBase::Notify() {
  {
    std::lock_guard<std::mutex> l(mutex_);
    if (stop_) {
      return;
    }
  }

  cv_.notify_one();
}

void ExecutorBase::Stop() {
  {
    std::lock_guard<std::mutex> l(mutex_);
    if (stop_) {
      return;
    }

    stop_ = true;
  }
  cv_.notify_one();
  executor_thread_.join();
}

void ExecutorBase::Loop() {
  while (true) {
    std::unique_lock<std::mutex> lk(mutex_);
    cv_.wait(lk);

    {
      if (stop_) {
        return;
      }
      std::lock_guard<std::shared_mutex> l(execution_handler_.commands_mutex_);
      if (!execution_handler_.commands_.empty()) {
        Execute(execution_handler_.commands_);
      }
    }
  }
}

ExecutorLogger::ExecutorLogger(ExecutorHandler& execution_handler)
    : ExecutorBase(execution_handler) {}

ExecutorLogger::~ExecutorLogger() {
  Stop();
}

void ExecutorLogger::Execute(ExecutorContentContainer& content_container) {
  int ready_flag = 0;
  for (auto& ec : content_container) {
    if (!ec->log_flag.compare_exchange_strong(ready_flag, 1)) {
      continue;
    }

    const auto& commands = ec->commands;
    if (!commands.empty()) {
      std::cout << "bulk: ";
      std::cout << *commands.begin();
      std::for_each(std::next(commands.begin()), commands.end(), [] (auto cmd) {
        std::cout << ", " << cmd;
      });

      std::cout << '\n';
    }

    ec->log_flag = 2;
  }
}

ExecutorFileWriter::ExecutorFileWriter(ExecutorHandler& execution_handler, int idx)
    : ExecutorBase(execution_handler)
    , idx_(idx) {}

ExecutorFileWriter::~ExecutorFileWriter() {
  Stop();
}

void ExecutorFileWriter::Execute(ExecutorContentContainer& content_container) {
  int ready_flag = 0;
  for (auto& ec : content_container) {
    if (!ec->write_flag.compare_exchange_strong(ready_flag, 1)) {
      continue;
    }

    const auto& commands = ec->commands;

    if (!commands.empty()) {
      std::ofstream file_stream;
      auto command_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch()).count();
      std::string filename =
          std::string("bulk_") +
          std::to_string(command_timestamp) +
          std::string("_") +
          std::to_string(idx_) +
          std::string(".log");
      file_stream.open(filename);
      file_stream << *commands.begin();
      std::for_each(std::next(commands.begin()), commands.end(), [&] (auto cmd) {
        file_stream << ", ";
        file_stream << cmd;
      });

      file_stream.close();
    }

    ec->write_flag = 2;
  }
}

ExecutorHandler::ExecutorHandler() {
  executors_.push_back(std::make_shared<ExecutorLogger>(*this));
  executors_.push_back(std::make_shared<ExecutorFileWriter>(*this, 1));
  executors_.push_back(std::make_shared<ExecutorFileWriter>(*this, 2));
}

void ExecutorHandler::ExecuteAndFlushCommands(BulkContext& bulk_context) {
  {
    std::lock_guard<std::shared_mutex> l(commands_mutex_);
    commands_.remove_if([](const auto& c) {
      return c->log_flag >= 2 && c->write_flag >= 2;
    });
      auto executor_content = std::make_shared<ExecutorContent>();
      executor_content->commands = std::move(bulk_context.commands);
      commands_.emplace_back(executor_content);
  }

  for (const auto& executor : executors_) {
    executor->Notify();
  }
}

void ExecutorHandler::Stop() {
  for (const auto& executor : executors_) {
    executor->Stop();
  }
}


const std::map<std::string, std::function<void(BulkContext&, ExecutorHandler&)>> CommandExecutor::kCommandMap = {
  {
    "{", [] (BulkContext& bulk_context, ExecutorHandler& executor_handler) {
      if (++bulk_context.block_count == 1) {
        executor_handler.ExecuteAndFlushCommands(bulk_context);
      }
    }
  },
  {
    "}", [] (BulkContext& bulk_context, ExecutorHandler& executor_handler) {
      if (--bulk_context.block_count == 0) {
        executor_handler.ExecuteAndFlushCommands(bulk_context);
      }

      bulk_context.block_count = std::max(0, bulk_context.block_count);
    }
  },
  {
    "EOF", [] (BulkContext& bulk_context, ExecutorHandler& executor_handler) {
      if (bulk_context.block_count == 0) {
        executor_handler.ExecuteAndFlushCommands(bulk_context);
      }

      bulk_context.eof = true;
    },
  },
};

void CommandExecutor::ExecuteCommand(
    const std::string& command,
    BulkContext& bulk_context,
    ExecutorHandler& executor_handler) {
  auto command_it = CommandExecutor::kCommandMap.find(command);
  if (command_it == CommandExecutor::kCommandMap.end()) {
    bulk_context.commands.push_back(command);
    if (bulk_context.commands.size() >= bulk_context.kBlockSize && bulk_context.block_count == 0) {
      executor_handler.ExecuteAndFlushCommands(bulk_context);
    }
  } else {
    command_it->second(bulk_context, executor_handler);
  }
}

} // namespace homework_09
