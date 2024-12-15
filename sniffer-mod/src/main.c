#include "include/db.h"
#include "include/flow.h"
#include "include/sniffer.h"
#include <stdio.h>

int main() {
  char *home_dir = get_home_dir();
  connect_db(home_dir);
  testFunc();

  start_multithreaded_sniffing();
  return 0;
}
