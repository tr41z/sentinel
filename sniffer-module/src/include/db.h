#ifndef DB_H
#define DB_H

#ifdef __cplusplus

#include "flow.h"
#include <sqlite3.h>

void save_flow(sqlite3 *db, const Flow &flow);

extern "C" {
#endif

#include <pwd.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void connect_db(const char *home_dir, sqlite3 **db);
void flows_table_build(int rc, sqlite3 *db);
static inline char *get_home_dir() {
  struct passwd *pw = getpwuid(getuid());
  char *homedir = pw->pw_dir;

  return homedir;
}
static inline void create_hidden_dir(char *dir) {
  struct stat st = {0};

  if (stat(dir, &st) == -1) {
    mkdir(dir, 0700);
  }
}
static inline char *connect_string(const char *s1, const char *s2) {
  char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
  if (!result) {
    fprintf(stderr, "Failed to allocate memory for `result`\n");
  }

  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

#ifdef __cplusplus
}
#endif

#endif
