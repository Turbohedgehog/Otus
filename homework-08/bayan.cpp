#include "bayan.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <regex>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>


namespace homework_08 {

class HashValue {
 public:
  HashValue(const HashValue&) = default;
  HashValue(HashValue&&) = default;
  HashValue(std::initializer_list<std::size_t> value)
      : value_(value) {}

  bool operator< (const HashValue& other) const {
    for (std::size_t i = 0; i < value_.size() && i < other.value_.size(); ++i) {
      if (value_[i] >= other.value_[i]) {
        return false;
      }
    }

    return true;
  }

 private:
  std::vector<std::size_t> value_;
};

struct FileChunkInfo {
  std::set<std::filesystem::path> paths;
  std::map<HashValue, FileChunkInfo> child_chunks;
};

Bayan::Bayan(int argc, char *argv[]) {
  is_valid_ = ParseConfig(argc, argv);
}

bool Bayan::ParseConfig(int argc, char *argv[]) {
  namespace po = boost::program_options;

  static const std::map<std::string, homework_08::Bayan::HashAlgorithm> kHashAlgorithmMap = {
    {"CRC32", homework_08::Bayan::HashAlgorithm::crc32},
    {"MD5", homework_08::Bayan::HashAlgorithm::md5},
  };

  po::options_description desc("Allowed options");
  std::size_t scan_depth;
  std::string hash_algorithm;
  desc.add_options()
    ("help,h", "Show help")
    ("dirs,d", po::value<std::vector<std::string>>(&dirs_)->multitoken()->default_value(std::vector<std::string>{"."}, "."), "Sets dirs to search")
    ("ignore-dirs,i", po::value<std::vector<std::string>>(&ignore_dirs_)->multitoken(), "Sets ignore dirs")
    ("scan-depth,s", po::value<std::size_t>(&scan_depth), "Scan depth (if not set then no limit)")
    ("min-size,m", po::value<std::size_t>(&min_file_size_)->default_value(1), "Min file size for scan in bytes")
    ("name-masks,n", po::value<std::vector<std::string>>(&filename_masks_)->multitoken(), "Regex filename masks")
    ("block-size,b", po::value<std::size_t>(&block_size_)->default_value(1024), "Hash block size")
    ("hash-algorithm,a", po::value<std::string>(&hash_algorithm)->default_value("crc32"), "Hash algorithm [crc32, md5]")
    ;

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch (po::error& e) {
    std::cerr << "Error: " << e.what() << std::endl;

    return false;
  }

  if (vm.count("help")) {
    std::cout << desc << "\n";

    return false;
  }

  boost::to_upper(hash_algorithm);
  const auto it = kHashAlgorithmMap.find(hash_algorithm);
  if (it == kHashAlgorithmMap.end()) {
    std::cerr << "Error: Hash algorithm '" << hash_algorithm << "' is not supported" << std::endl;

    return false;
  }

  hash_algorithm_ = it->second;

  if (vm.count("scan-depth")) {
    scan_depth_ = scan_depth;
  }

  return true;
}

bool Bayan::IsValid() const {
  return is_valid_;
}

std::map<std::size_t, std::set<std::filesystem::path>> Bayan::CollectFilenames() const {
  std::map<std::size_t, std::set<std::filesystem::path>> file_infos;

  std::vector<std::regex> masks;
  std::transform(
      filename_masks_.begin(),
      filename_masks_.end(),
      std::back_inserter(masks),
      [](const std::string& mask) {
        return std::regex(mask);
      });

  auto current_path = std::filesystem::current_path();

  auto concat_dirs = [&](const std::vector<std::string>& dirs) {
    std::set<std::filesystem::path> res_dirs;
    std::transform(
      dirs.begin(),
      dirs.end(),
      std::inserter(res_dirs, res_dirs.end()),
      [&](const std::string& dir) {
        return std::filesystem::weakly_canonical(current_path / dir);
      });

    return res_dirs;
  };

  auto dirs_to_scan = concat_dirs(dirs_);
  auto ignore_dirs = concat_dirs(ignore_dirs_);

  std::set<std::filesystem::path> scanned_paths;
  std::function<void(const std::filesystem::path&)> scan_dirs = [&](const std::filesystem::path& path) {
    if (scanned_paths.count(path) > 0) {
      return;
    }

    scanned_paths.insert(path);

    if (!std::filesystem::exists(path)) {
      return;
    }

    for (const auto& ignore_dir : ignore_dirs) {
      auto relative_ignore = std::filesystem::relative(ignore_dir, path);
      if (relative_ignore.empty() || relative_ignore.native()[0] == '.') {
        return;
      }
    }

    if (scan_depth_) {
      auto relative = std::filesystem::relative(path, current_path);

      std::size_t d = 0;
      for (const auto& _ : relative) {
        if (d++ >= scan_depth_.value()) {
          return;
        }
      }
    }

    std::smatch base_match;
    
    for (const auto& dir_entry : std::filesystem::directory_iterator(path)) {
      if (!dir_entry.is_directory() && !dir_entry.is_regular_file()) {
        continue;
      }

      const auto& entry_path = dir_entry.path();
      if (scanned_paths.count(entry_path) > 0) {
        continue;
      }

      scanned_paths.insert(entry_path);

      if (dir_entry.is_regular_file()) {
        if (std::none_of(masks.begin(), masks.end(), [&](const std::regex& r) {
          return std::regex_search(entry_path.string(), r);
        })) {
          continue;
        }

        auto file_size = static_cast<std::size_t>(dir_entry.file_size());
        if (file_size >= min_file_size_) {
          file_infos[file_size].insert(entry_path);
        }
      } else {
        scan_dirs(entry_path);
      }
    }
  };

  for (const auto& dir_to_scan : dirs_to_scan) {
    scan_dirs(dir_to_scan);
  }

  for (auto it = file_infos.begin(); it != file_infos.end();) {
    if (it->second.size() < 2) {
      it = file_infos.erase(it);
    } else {
      ++it;
    }
  }

  return file_infos;
}

using HashFunction = std::function<HashValue(const std::vector<char>&)>;

std::optional<HashValue> ScanChunk(
    const std::filesystem::path& path,
    std::size_t block_size,
    std::size_t block_index,
    const HashFunction& hasher) {
  if (block_size == 0) {
    return std::nullopt;
  }

  std::ifstream is{path, std::ios::in | std::ios::binary | std::ios::ate};
  std::size_t file_size = static_cast<std::size_t>(is.tellg());
  std::size_t cur_pos = block_size * block_index;
  if (cur_pos >= file_size) {
    return std::nullopt;
  }

  std::vector<char> chunk(block_size);
  is.seekg(cur_pos, is.beg);
  std::size_t chunk_end = block_size * (block_index + 1);
  std::size_t read_len = cur_pos + block_size > file_size ? file_size - cur_pos : block_size;
  is.read(chunk.data(), read_len);

  return hasher(chunk);
}

void ScanChunkInfo(
    FileChunkInfo& chunk_info,
    std::size_t block_size,
    std::size_t block_index,
    const HashFunction& hasher) {
  auto& child_chunks = chunk_info.child_chunks;

  for (const auto& path : chunk_info.paths) {
    auto hash_info = ScanChunk(path, block_size, block_index, hasher);
    if (!hash_info) {
      continue;
    }

    auto& child_chunk = child_chunks[hash_info.value()];
    child_chunk.paths.insert(path);
  }

  for (auto it = child_chunks.begin(); it != child_chunks.end();) {
    if (it->second.paths.size() < 2) {
      it = child_chunks.erase(it);
    } else {
      ScanChunkInfo(it->second, block_size, block_index + 1, hasher);
      ++it;
    }
  }
}

void ExtractBayanFiles(
    const FileChunkInfo& chunk_info,
    std::vector<std::set<std::filesystem::path>>& bayans,
    bool is_root_chunk = false) {
  if (chunk_info.child_chunks.empty() && !is_root_chunk) {
    bayans.push_back(chunk_info.paths);

    return;
  }

  for (const auto& child_chunk_pair : chunk_info.child_chunks) {
    ExtractBayanFiles(child_chunk_pair.second, bayans, false);
  }
}

HashValue Crc32Hash(const std::vector<char>& chunk) {
  boost::crc_32_type result;
  result.process_bytes(chunk.data(), chunk.size());

  return HashValue({result.checksum()});
}

HashValue Md5Hash(const std::vector<char>& chunk) {
  boost::uuids::detail::md5 hash;
  boost::uuids::detail::md5::digest_type digest;

  hash.process_bytes(chunk.data(), chunk.size());
  hash.get_digest(digest);

  return HashValue({
    digest[0],
    digest[1],
    digest[2],
    digest[3],
  });
}

void Bayan::Scan() {
  std::vector<std::set<std::filesystem::path>> bayans;
  HashFunction haser;
  switch (hash_algorithm_) {
  case Bayan::HashAlgorithm::md5:
    haser = &Md5Hash;
    break;
  default:
    haser = &Crc32Hash;
    break;
  }

  auto file_infos = CollectFilenames();
  for (auto it = file_infos.begin(); it != file_infos.end(); ++it) {
    const auto& paths = it->second;
    FileChunkInfo root_chunk;
    root_chunk.paths = paths;
    ScanChunkInfo(root_chunk, block_size_, 0, haser);
    ExtractBayanFiles(root_chunk, bayans, true);
  }

  for (const auto& bayan_group : bayans) {
    std::cout << "Bayan files group:\n";
    for (const auto& path : bayan_group) {
      std::cout << path.string() << '\n';
    }

    std::cout << '\n';
  }

  if (bayans.empty()) {
    std::cout << "No bayan files found!\n";
  }
}

}  // namespace homework_08
