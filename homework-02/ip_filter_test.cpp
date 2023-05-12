#include <gtest/gtest.h>

#include "ip_filter.h"

namespace {

TEST(test_ip_filter, test_valid_value) {
  try {
    homework_02::IPFilter ip_filter;
    ip_filter.AddAddress("1.1.1.1\t1\t1");
    ip_filter.AddAddress("1.2.3.4\t1\t1");

    EXPECT_EQ(ip_filter.GetFiltered().size(), 2);

    ip_filter.Filter(1);
    EXPECT_EQ(ip_filter.GetFiltered().size(), 2);

    ip_filter.Filter(1, 2);
    EXPECT_EQ(ip_filter.GetFiltered().size(), 1);

    ip_filter.Filter(1, 0);
    EXPECT_EQ(ip_filter.GetFiltered().size(), 0);

    ip_filter.FilterAny(1);
    EXPECT_EQ(ip_filter.GetFiltered().size(), 2);

    ip_filter.FilterAny(10);
    EXPECT_EQ(ip_filter.GetFiltered().size(), 0);

  } catch (const std::exception& e) {
    FAIL() << e.what();
  }
}

TEST(test_ip_filter, test_wrong_value_1) {
  try {
    homework_02::IPFilter ip_filter;
    ip_filter.AddAddress("abc");
    FAIL() << "Expected runtime_error";
  } catch (const std::runtime_error& runtime_ex) {
    EXPECT_EQ(runtime_ex.what(), std::string("Cannot parse expression"));
  } catch (...) {
    FAIL() << "Expected runtime_error";
  }
}

TEST(test_ip_filter, test_wrong_value_2) {
  try {
    homework_02::IPFilter ip_filter;
    ip_filter.AddAddress("1.2.3.4 5 6");
    FAIL() << "Expected runtime_error";
  } catch (const std::runtime_error& runtime_ex) {
    EXPECT_EQ(runtime_ex.what(), std::string("Cannot parse expression"));
  } catch (...) {
    FAIL() << "Expected runtime_error";
  }
}

TEST(test_ip_filter, test_wrong_value_3) {
  try {
    homework_02::IPFilter ip_filter;
    ip_filter.AddAddress("500.2.3.4\t5\t6");
    FAIL() << "Expected runtime_error";
  } catch (const std::runtime_error& runtime_ex) {
    EXPECT_EQ(runtime_ex.what(), std::string("The ip segment value must be between 0 and 255"));
  } catch (...) {
    FAIL() << "Expected runtime_error";
  }
}

}  // namespace
