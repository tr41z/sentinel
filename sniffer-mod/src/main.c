#include "include/db.h"
#include "include/sniffer.h"
#include <stdio.h>

int main() {
  char *home_dir = get_home_dir();
  devPtr dev = find_devices();

  connect_db(home_dir);
  if (!dev) {
    printf("No devices found or an error occured!\n");
    return 1;
  }

  start_sniffer(dev);
  return 0;
}
