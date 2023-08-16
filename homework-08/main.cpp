#include "bayan.h"


int main (int argc, char *argv[]) {
  homework_08::Bayan b(argc, argv);
  if (b.IsValid()) {
    b.Scan();
  }

  return 0;
}