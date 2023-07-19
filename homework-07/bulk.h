#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>

namespace homework_07 {

class BulkParser;

class BulkExecutorBase {
 public:
  virtual void AcceptCommand(const BulkParser&) = 0;
};

class BulkPrinter : public BulkExecutorBase {
 public:
  void AcceptCommand(const BulkParser& bulk_parser) override;
};

class BulkFileWriter : public BulkExecutorBase {
 public:
  void AcceptCommand(const BulkParser& bulk_parser) override;
};

using BulkExecutorPtr = std::shared_ptr<BulkExecutorBase>;

class BulkParser {
 public:
  BulkParser(std::size_t block_size = 3);
  void Run();
  void AppendBulkExecutor(BulkExecutorPtr bulk_executor);

  template<typename T>
  void AppendBulkExecutor() {
    AppendBulkExecutor(std::make_shared<T>());
  }

  unsigned long long GetCommandTimestamp() const;
  const std::vector<std::string>& GetCommands() const;

 private:
  void ExecuteAndFlushCommands();

  const std::size_t block_size_;
  std::vector<BulkExecutorPtr> bulk_executors_;
  bool eof_ = false;
  std::vector<std::string> commands_;
  unsigned long long command_timestamp_ = 0;
  int block_counter_ = 0;
  std::map<std::string, std::function<void()>> command_map_;
};

}  // namespace homework_07
