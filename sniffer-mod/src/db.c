#include "include/db.h"

void connect_db(char *home_dir) {
  sqlite3 *db;
  int rc;
  char *connection_string;
  char *hidden_dir;

  hidden_dir = connect_string(home_dir, "/.sentinel/");
  create_hidden_dir(hidden_dir);

  connection_string = connect_string(home_dir, "/.sentinel/sentinel.db");
  printf("CONNECTION STRING: %s", connection_string);
  rc = sqlite3_open(connection_string, &db);

  if (rc) {
    fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

  sqlite3_close(db);
  free(connection_string);
  free(hidden_dir);
}
