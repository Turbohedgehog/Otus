#include <atomic>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <condition_variable>

namespace homework_09 {

struct BulkContext {
 public:
  BulkContext(std::size_t block_size = 3);

  const std::size_t kBlockSize;
  bool eof = false;
  std::vector<std::string> commands;
  int block_count = 0;

};

struct ExecutorContent {
  std::atomic_int log_flag = 0;
  std::atomic_int write_flag = 0;
  std::vector<std::string> commands;
};

using ExecutorContentContainer = std::list<std::shared_ptr<ExecutorContent>>;

class ExecutorBase {
 public:
  ExecutorBase(class ExecutorHandler& execution_handler);

  virtual ~ExecutorBase();
  void Notify();
  void Stop();

 protected:
  class ExecutorHandler& execution_handler_;

 private:
  void Loop();
  virtual void Execute(ExecutorContentContainer& content_container) = 0;

  bool stop_ = false;
  std::mutex mutex_;
  std::thread executor_thread_;
  std::condition_variable cv_;
  
};

class ExecutorLogger : public ExecutorBase {
 public:
  ExecutorLogger(class ExecutorHandler& execution_handler);
  ~ExecutorLogger();

 private:
  void Execute(ExecutorContentContainer& content_container) override;
};

class ExecutorFileWriter : public ExecutorBase {
 public:
  ExecutorFileWriter(class ExecutorHandler& execution_handler, int idx);
  ~ExecutorFileWriter();

 private:
  void Execute(ExecutorContentContainer& content_container) override;

  int idx_ = 0;

};

class ExecutorHandler {
  friend class ExecutorBase;

 public:
  ExecutorHandler();
  void ExecuteAndFlushCommands(BulkContext& bulk_context);
  void Stop();

 private:
  ExecutorContentContainer commands_;
  std::shared_mutex commands_mutex_;
  std::vector<std::shared_ptr<ExecutorBase>> executors_;
};

class CommandExecutor {
 public:
  static void ExecuteCommand(
      const std::string& command,
      BulkContext& bulk_context,
      ExecutorHandler& executor_handler);

 private:
  static const std::map<std::string, std::function<void(BulkContext&, ExecutorHandler&)>> kCommandMap;
};

}  // namespace homework_07
