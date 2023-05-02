#include <gtest/gtest.h>

#include "lib.h"

namespace {

TEST(test_version, test_valid_version) {
  ASSERT_TRUE(HomeWork01::version() > 0);
}

}  // namespace
