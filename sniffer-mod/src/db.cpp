#include "include/db.h"
#include "include/flow.h"
#include "include/ip.h"

#ifdef _WIN32
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

void connect_db(const char *home_dir, sqlite3 **db) {
  int rc;
  char *connection_string;
  char *hidden_dir;

  // Use the platform-specific path separator
  hidden_dir = connect_string(home_dir, PATH_SEPARATOR ".sentinel" PATH_SEPARATOR);
  create_hidden_dir(hidden_dir);

  connection_string = connect_string(home_dir, PATH_SEPARATOR ".sentinel" PATH_SEPARATOR "sentinel.db");
  rc = sqlite3_open(connection_string, db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(*db));
    sqlite3_close(*db); // Ensure resources are cleaned up
    *db = nullptr;
  } else {
    std::cout << "Successfully connected to database.\n";
    flows_table_build(rc, *db);
  }

  // Ensure resources are cleaned up in case of an error
  if (rc != SQLITE_OK) {
    sqlite3_close(*db);
    *db = nullptr;
  }

  free(connection_string);
  free(hidden_dir);
}

void save_flow(sqlite3 *db, const Flow &flow) {
  sqlite3_stmt *stmt;
  const char *insert_sql =
      "INSERT INTO flows (src_ip, dst_ip, src_ports, dst_ports, protocol, total_bytes, rate,"
      "avg_packet_size, total_packet_count, start_time, last_updated_time, duration) "
      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

  int rc = sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return;
  }

  // Ensure duration and packet_count are not zero to avoid division by zero
  double flow_rate = (flow.duration.count() > 0) ? 
                     static_cast<double>(flow.total_bytes) / flow.duration.count() : 0.0;
  double flow_avg_packet_size = (flow.packet_count > 0) ? 
                                static_cast<double>(flow.total_bytes) / flow.packet_count : 0.0;

  // Convert port sets to comma-separated strings
  std::string src_ports_str;
  for (const auto &port : flow.src_ports) {
    src_ports_str += std::to_string(port) + ",";
  }
  if (!src_ports_str.empty()) {
    src_ports_str.pop_back();
  }

  std::string dst_ports_str;
  for (const auto &port : flow.dst_ports) {
    dst_ports_str += std::to_string(port) + ",";
  }
  if (!dst_ports_str.empty()) {
    dst_ports_str.pop_back(); 
  }

  // Bind actual flow values
  sqlite3_bind_text(stmt, 1, ip_to_str(flow.src_ip).c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, ip_to_str(flow.dst_ip).c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, src_ports_str.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, dst_ports_str.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 5, flow.protocol);
  sqlite3_bind_int(stmt, 6, flow.total_bytes);
  sqlite3_bind_double(stmt, 7, flow_rate);
  sqlite3_bind_double(stmt, 8, flow_avg_packet_size);
  sqlite3_bind_int(stmt, 9, flow.packet_count);
  sqlite3_bind_int64(stmt, 10, std::chrono::system_clock::to_time_t(flow.start_time));
  sqlite3_bind_int64(stmt, 11, std::chrono::system_clock::to_time_t(flow.last_update_time));
  sqlite3_bind_int(stmt, 12, flow.duration.count());

  // Execute the statement
  rc = sqlite3_step(stmt);
  if (rc == SQLITE_DONE) {
    std::cout << "Flow successfully saved to DB.\n";
  } else {
    fprintf(stderr, "Failed to insert data: %s (Code: %d)\n", sqlite3_errmsg(db), rc);
  }

  sqlite3_finalize(stmt);
}

void flows_table_build(int rc, sqlite3 *db) {
  const char sql[] =
      "CREATE TABLE IF NOT EXISTS flows("
      "id                    INTEGER          PRIMARY KEY       AUTOINCREMENT,"
      "src_ip                VARCHAR(45),"
      "dst_ip                VARCHAR(45),"
      "src_ports             TEXT,"
      "dst_ports             TEXT,"
      "protocol              INT,"
      "total_bytes           INT,"
      "rate                  FLOAT,"
      "avg_packet_size       FLOAT,"
      "total_packet_count    INT,"
      "start_time            INTEGER,"
      "last_updated_time     INTEGER,"
      "duration              INT);";

  rc = sqlite3_exec(db, sql, NULL, NULL, NULL);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cant create table: %s\n", sqlite3_errmsg(db));
  }
}
