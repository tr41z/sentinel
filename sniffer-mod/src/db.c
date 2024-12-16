#include "include/db.h"
#include <stdio.h>

void connect_db(char *home_dir) {
  sqlite3 *db;
  int rc;
  char *connection_string;
  char *hidden_dir;

  hidden_dir = connect_string(home_dir, "/.sentinel/");
  create_hidden_dir(hidden_dir);

  connection_string = connect_string(home_dir, "/.sentinel/sentinel.db");
  printf("CONNECTION STRING: %s\n", connection_string);
  rc = sqlite3_open(connection_string, &db);

  if (rc) {
    fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Opened database successfully\n");
    flows_table_build(rc, db);
  }

  sqlite3_close(db);
  free(connection_string);
  free(hidden_dir);
}

void flows_table_build(int rc, sqlite3 *db) {
  const char sql[] =
      "CREATE TABLE IF NOT EXISTS FLOWS("
      "id                    INTEGER          PRIMARY KEY       AUTOINCREMENT,"
      "src_ip                VARCHAR(45),"
      "src_port              INT,"
      "dst_ip                VARCHAR(45),"
      "dst_port              INT,"
      "protocol              INT,"
      "total_bytes           INT,"
      "total_packet_count    INT,"
      "start_time            INTEGER,"
      "last_updated_time     INTEGER);";

  rc = sqlite3_exec(db, sql, NULL, NULL, NULL);

  if (rc) {
    fprintf(stderr, "Cant create table: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Table created successfully!\n");
  }
}
