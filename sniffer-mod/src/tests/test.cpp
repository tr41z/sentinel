#include "../include/db.h"
#include "../include/packet.h"
#include "../include/sniffer.h"
#include <gtest/gtest.h>
#include <string>

TEST(StringConnectionTest, BasicConcatenation) {
  std::string result = connect_string("Hello ", "World!");
  EXPECT_EQ(result, "Hello World!");
}

TEST(StringConnectionTest, ConcatenationWithPath) {
  std::string home_dir = "/Users/profile";
  std::string db_path = "/.sentinel/sentinel.db";
  std::string result = connect_string(home_dir.c_str(), db_path.c_str());
  EXPECT_EQ(result, "/Users/profile/.sentinel/sentinel.db");
}

TEST(StringConnectionTest, EdgeCaseEmptyStrings) {
  std::string result = connect_string("", "");
  EXPECT_EQ(result, "");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
