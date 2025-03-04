#include "include/db.h"
#include "include/flow.h"
#include "include/ip.h"
#include "include/prep.h"

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
    free(connection_string);
    free(hidden_dir);
    return;
  } else {
    std::cout << "Successfully connected to database.\n";
    flows_table_build(rc, *db);
  }

  free(connection_string);
  free(hidden_dir);
}

void save_flow(sqlite3 *db, const Flow &flow) {
  sqlite3_stmt *stmt;
  const char *insert_sql =
      "INSERT INTO flows (src_ip, dst_ip, src_ports, dst_ports, protocol, total_bytes, rate,"
      "avg_packet_size, total_packet_count, src_system_ports_count, src_registered_ports_count, src_dynamic_ports_count, dst_system_ports_count, dst_registered_ports_count, dst_dynamic_ports_count, packets_per_sec, is_brute_target, is_dos_target, start_time, last_updated_time, duration) "
      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

  int rc = sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return;
  }

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

  int src_system_ports_count = count_ports_in_range(flow.src_ports, 0, 1023);
  int src_registered_ports_count = count_ports_in_range(flow.src_ports, 1024, 49151);
  int src_dynamic_ports_count = count_ports_in_range(flow.src_ports, 49152, 65535);
  int dst_system_ports_count = count_ports_in_range(flow.dst_ports, 0, 1023);
  int dst_registered_ports_count = count_ports_in_range(flow.dst_ports, 1024, 49151);
  int dst_dynamic_ports_count = count_ports_in_range(flow.dst_ports, 49152, 65535);

  double flow_rate = calculate_rate(flow.total_bytes, flow.duration.count());
  double flow_avg_packet_size = calculate_bpp(flow.total_bytes, flow.packet_count);
  double pps = calculate_pps(flow.packet_count, flow.duration.count());

  int is_brute = is_brute_target(flow.dst_ports);
  int is_dos = is_dos_target(flow.dst_ports);
  
  // Bind actual flow values
  sqlite3_bind_text(stmt, 1, ip_to_str(flow.src_ip).c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, ip_to_str(flow.dst_ip).c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, src_ports_str.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, dst_ports_str.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 5, flow.protocol);
  sqlite3_bind_int(stmt, 6, flow.total_bytes);
  sqlite3_bind_double(stmt, 7, flow_rate);
  sqlite3_bind_double(stmt, 8, flow_avg_packet_size);
  sqlite3_bind_int(stmt, 9, flow.packet_count);
  sqlite3_bind_int(stmt, 10, src_system_ports_count);
  sqlite3_bind_int(stmt, 11, src_registered_ports_count);
  sqlite3_bind_int(stmt, 12, src_dynamic_ports_count);
  sqlite3_bind_int(stmt, 13, dst_system_ports_count);
  sqlite3_bind_int(stmt, 14, dst_registered_ports_count);
  sqlite3_bind_int(stmt, 15, dst_dynamic_ports_count);
  sqlite3_bind_double(stmt, 16, pps);
  sqlite3_bind_int(stmt, 17, is_brute);
  sqlite3_bind_int(stmt, 18, is_dos);
  sqlite3_bind_int64(stmt, 19, std::chrono::system_clock::to_time_t(flow.start_time));
  sqlite3_bind_int64(stmt, 20, std::chrono::system_clock::to_time_t(flow.last_update_time));
  sqlite3_bind_int(stmt, 21, flow.duration.count());

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
      "src_system_ports_count INT,"
      "src_registered_ports_count INT,"
      "src_dynamic_ports_count INT,"
      "dst_system_ports_count INT,"
      "dst_registered_ports_count INT,"
      "dst_dynamic_ports_count INT,"
      "packets_per_sec INT,"
      "is_brute_target INT,"
      "is_dos_target INT,"
      "start_time            INTEGER,"
      "last_updated_time     INTEGER,"
      "duration              INT);";

  rc = sqlite3_exec(db, sql, NULL, NULL, NULL);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cant create table: %s\n", sqlite3_errmsg(db));
  }
}
