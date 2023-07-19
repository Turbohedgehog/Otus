
#include "bulk.h"

#include <iostream>

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

  auto bulk_parser = homework_07::BulkParser(block_size);
  bulk_parser.AppendBulkExecutor<homework_07::BulkPrinter>();
  bulk_parser.AppendBulkExecutor<homework_07::BulkFileWriter>();

  bulk_parser.Run();

  return 0;
}