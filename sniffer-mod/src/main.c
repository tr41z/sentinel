#include "include/sniffer.h"
#include <stdio.h>

int main() {
  interPtr dev = find_devices();

  if (dev == NULL) {
    printf("No devices found or an error occured!\n");
    return 1;
  }

  start_sniffer(dev);
  return 0;
}
