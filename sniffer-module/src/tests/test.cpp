#include "../include/db.h"
#include "../include/packet.h"
#include "../include/sniffer.h"
#include <gtest/gtest.h>
#include <sqlite3.h>
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

TEST(DBConnectionTest, SuccessfulConnection) {
  sqlite3 *db = nullptr; // Initialize db to nullptr
  char *home_dir = get_home_dir();

  EXPECT_NO_THROW(connect_db(home_dir, &db));
}

TEST(DBConnectionTest, FailedConnection) {
  sqlite3 *db = nullptr;
  const char *invalid_path = "/asdlka/dkwo"; // Invalid path

  // Capture stderr
  testing::internal::CaptureStderr();
  connect_db(invalid_path, &db); // Passing invalid path to trigger error
  std::string output = testing::internal::GetCapturedStderr();

  // Verify the database connection failed
  EXPECT_EQ(db, nullptr); // db should remain nullptr
  EXPECT_NE(output.find("Cant open database:"), std::string::npos)
      << "Expected error message not found in stderr";
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
