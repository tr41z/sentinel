#include "include/sniffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char err_buff[PCAP_ERRBUF_SIZE]; /* Error string */
pcap_t *handle;                  /* Session handle */
pcap_if_t *interface, *temp;     /* Interfaces */

devPtr find_devices() {
  // Find all devices
  if (pcap_findalldevs(&interface, err_buff) == -1) {
    printf("\nError in pcap_findalldevs: %s\n", err_buff);
    return NULL;
  }

  // Iterate through interfaces and find en0 for now
  for (temp = interface; temp; temp = temp->next) {
    if (strcmp(temp->name, "en0") == 0) {
      printf("Found device: %s\n", temp->name);
      return temp; /* Return en0 if found */
    }
  }

  // If en0 is not found
  printf("Device en0 not found!\n");
  return NULL;
}

void start_sniffer(devPtr dev) {
  if (!dev) {
    fprintf(stderr, "Device pointer is NULL. Cannot start sniffer.\n");
    return;
  }

  handle = pcap_open_live(dev->name, BUFSIZ, 1, 5, err_buff);
  if (!handle) {
    fprintf(stderr, "Couldn't open device %s: %s\n", dev->name, err_buff);
    return;
  }

  printf("Listening on device: %s\n", dev->name);

  pcap_loop(handle, 0, packet_handler, NULL);
  pcap_close(handle); /* Close the handle when done */

  free(handle);
  handle = NULL;
}
