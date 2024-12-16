#include "include/db.h"
#include "include/sniffer.h"
#include <stdio.h>

int main() {
  char *home_dir = get_home_dir();
  connect_db(home_dir);

  start_multithreaded_sniffing();
  return 0;
}
