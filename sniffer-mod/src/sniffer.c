#include "include/sniffer.h"
#include <stdio.h>
#include <string.h>

pcap_t *handle;                  /* Session handle */
char err_buff[PCAP_ERRBUF_SIZE]; /* Error string */
struct bpf_program fp;           /* The compiled filter */
bpf_u_int32 mask;                /* Netmask */
struct pcap_pkthdr header;       /* The header that pcap returns */
bpf_u_int32 net;                 /* IP Address */
const u_char *packet;            /* Actual packet */
pcap_if_t *interface, *temp;     /* Interfaces */

interPtr find_devices() {
  // Find all devices
  if (pcap_findalldevs(&interface, err_buff) == -1) {
    printf("\nError in pcap_findalldevs: %s\n", err_buff);
    return NULL;
  }

  // Iterate through interfaces and find en0 for now
  for (temp = interface; temp; temp = temp->next) {
    if (strcmp(temp->name, "en0") == 0) {
      printf("Found device: %s\n", temp->name);
      return temp; // Return en0 if found
    }
  }

  // If en0 is not found
  printf("Device en0 not found!\n");
  return NULL;
}

void start_sniffer(interPtr dev) {
  if (dev == NULL) {
    fprintf(stderr, "Device pointer is NULL. Cannot start sniffer.\n");
    return;
  }

  handle = pcap_open_live(dev->name, BUFSIZ, 1, 1000, err_buff);
  if (handle == NULL) {
    fprintf(stderr, "Couldn't open device %s: %s\n", dev->name, err_buff);
    return;
  }

  printf("Listening on device: %s\n", dev->name);

  while (1) {
    packet = pcap_next(handle, &header);
    if (packet == NULL) {
      const char *error = pcap_geterr(handle); // Get detailed error message
      if (error && strlen(error) > 0) {
        fprintf(stderr, "Failed to capture packet: %s\n", error);
      } else {
        fprintf(stderr, "Failed to capture packet. Unknown error.\n");
      }
      continue; // Skip to the next iteration
    }

    // Process the packet if successfully captured
    header_info(header);
  }

  pcap_close(handle); // Close the handle when done.
}
