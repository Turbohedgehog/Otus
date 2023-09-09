#include <vector>
#include <string>
#include <optional>
#include <set>
#include <map>
#include <filesystem>

namespace homework_08 {

class Bayan {
 public:
  enum class HashAlgorithm {
    crc32,
    md5,
  };

  Bayan(int argc, char *argv[]);

  bool IsValid() const;
  void Scan();

 private:
  bool ParseConfig(int argc, char *argv[]);
  std::map<std::size_t, std::set<std::filesystem::path>> CollectFilenames() const;

  bool is_valid_ = false;
  std::vector<std::string> dirs_;
  std::vector<std::string> ignore_dirs_;
  std::optional<std::size_t> scan_depth_;
  std::size_t min_file_size_ = 1;
  std::vector<std::string> filename_masks_;
  std::size_t block_size_ = 1024;
  HashAlgorithm hash_algorithm_ = HashAlgorithm::crc32;
};

}  // namespace homework_08
