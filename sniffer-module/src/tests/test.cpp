#include "../include/db.h"
#include "../include/flow.h"
#include "../include/packet.h"
#include "../include/sniffer.h"
#include <cstdint>
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
  sqlite3 *db = nullptr; // initialize db to nullptr
  char *home_dir = get_home_dir();

  EXPECT_NO_THROW(connect_db(home_dir, &db));
}

TEST(DBConnectionTest, FailedConnection) {
  sqlite3 *db = nullptr;
  const char *invalid_path = "/asdlka/dkwo"; // invalid path

  // Capture stderr
  testing::internal::CaptureStderr();
  connect_db(invalid_path, &db); // passing invalid path to trigger error
  std::string output = testing::internal::GetCapturedStderr();

  // Verify the database connection failed
  EXPECT_EQ(db, nullptr); // db should remain nullptr
  EXPECT_NE(output.find("Cant open database:"), std::string::npos)
      << "Expected error message not found in stderr";
}

TEST(FlowKeyCreationTest, ValidKey) {
  uint32_t src_ip = 1921686878;
  uint32_t dst_ip = 19216811;
  uint8_t protocol = 8;

  FlowKey result = create_normalized_key(src_ip, dst_ip, protocol);
  FlowKey valid = FlowKey{19216811, 1921686878, 8};

  EXPECT_EQ(result, valid);
}

TEST(FlowKeyCreationTest, NotValidKey) {
  uint32_t src_ip = 1921686878;
  uint32_t dst_ip = 19216811;
  uint8_t protocol = 8;

  FlowKey result = create_normalized_key(src_ip, dst_ip, protocol);
  FlowKey valid = FlowKey{1921686878, 19216811, 8}; // not sorted

  EXPECT_NE(result, valid);
}

TEST(FlowKeyCreationTest, SrcDstEqual) {
  uint32_t src_ip = 1921686878;
  uint32_t dst_ip = 1921686878;
  uint8_t protocol = 8;

  FlowKey result = create_normalized_key(src_ip, dst_ip, protocol);
  FlowKey valid = FlowKey{1921686878, 1921686878, 8};

  EXPECT_EQ(result, valid);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
