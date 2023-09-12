#include "async.h"

#include <map>
#include <shared_mutex>

#include "bulk.h"

namespace homework_09 {

uint32_t context_couter = 0;
std::map<uint32_t, BulkContext> context;
std::shared_mutex context_mutex;
homework_09::ExecutorHandler executor_handler;

uint32_t connect(uint32_t block_size) {
  std::lock_guard<std::shared_mutex> lock_write(context_mutex);
  ++context_couter;
  context.emplace(context_couter, BulkContext(block_size));
  return context_couter;
}

void receive(const char* buffer, uint32_t buffer_size, uint32_t context_id) {
  std::shared_lock<std::shared_mutex> lock_read(context_mutex);
  auto context_it = context.find(context_id);
  if (context_it == context.end()) {
    return;
  }

  homework_09::CommandExecutor::ExecuteCommand(
    std::string(buffer, buffer + buffer_size),
    context_it->second,
    executor_handler);
}

void disconnect(uint32_t context_id) {
  std::lock_guard<std::shared_mutex> lock_write(context_mutex);
  auto context_it = context.find(context_id);
  if (context_it != context.end()) {
    context.erase(context_it);
  }
}

}