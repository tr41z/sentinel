#include "include/db.h"
#include "include/ip.h"

void connect_db(char *home_dir, sqlite3 **db) {
  int rc;
  char *connection_string;
  char *hidden_dir;

  hidden_dir = connect_string(home_dir, "/.sentinel/");
  create_hidden_dir(hidden_dir);

  connection_string = connect_string(home_dir, "/.sentinel/sentinel.db");
  printf("CONNECTION STRING: %s\n", connection_string);
  rc = sqlite3_open(connection_string, db);

  if (rc) {
    fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(*db));
  } else {
    fprintf(stderr, "Opened database successfully\n");
    flows_table_build(rc, *db);
  }

  free(connection_string);
  free(hidden_dir);
}

void save_flow(sqlite3 *db, const Flow &flow) {
  sqlite3_stmt *stmt;
  const char *insert_sql =
      "INSERT INTO FLOWS (src_ip, dst_ip, protocol, total_bytes, "
      "total_packet_count, "
      "src_port_count, dst_port_count, start_time, last_updated_time) "
      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

  int rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return;
  }

  // Bind values
  sqlite3_bind_text(stmt, 1, ip_to_str(flow.src_ip).c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, ip_to_str(flow.dst_ip).c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 3, flow.protocol);
  sqlite3_bind_int(stmt, 4, flow.total_bytes);
  sqlite3_bind_int(stmt, 5, flow.packet_count);
  sqlite3_bind_int(stmt, 6, flow.src_port_count);
  sqlite3_bind_int(stmt, 7, flow.dst_port_count);
  sqlite3_bind_int64(stmt, 8,
                     std::chrono::system_clock::to_time_t(flow.start_time));
  sqlite3_bind_int64(
      stmt, 9, std::chrono::system_clock::to_time_t(flow.last_update_time));

  // Execute the statement
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to insert data: %s\n", sqlite3_errmsg(db));
  } else {
    std::cout << "Flow saved to DB.\n";
  }

  sqlite3_finalize(stmt);
}

void flows_table_build(int rc, sqlite3 *db) {
  const char sql[] =
      "CREATE TABLE IF NOT EXISTS FLOWS("
      "id                    INTEGER          PRIMARY KEY       AUTOINCREMENT,"
      "src_ip                VARCHAR(45),"
      "dst_ip                VARCHAR(45),"
      "protocol              INT,"
      "total_bytes           INT,"
      "total_packet_count    INT,"
      "src_port_count     INT,"
      "dst_port_count        INT,"
      "start_time            INTEGER,"
      "last_updated_time     INTEGER);";

  rc = sqlite3_exec(db, sql, NULL, NULL, NULL);

  if (rc) {
    fprintf(stderr, "Cant create table: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Table created successfully!\n");
  }
}
