#include "include/sniffer.h"
#include <net/ethernet.h>
#include <string.h>

pcap_t *handle;                  /* Session handle */
char err_buff[PCAP_ERRBUF_SIZE]; /* Error string */
struct bpf_program fp;           /* The compiled filter */
struct pcap_pkthdr header;       /* The header that pcap returns */
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

void packet_handler(u_char *args, const struct pcap_pkthdr *header,
                    const u_char *packet) {

  /* Finding IP Packets */
  struct ether_header *eth_header;
  eth_header = (struct ether_header *)packet;
  if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
    printf("Not an IP packet. Skipping....\n\n");
    return;
  }
  // Process the packet if successfully captured
  header_info(header);
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

  pcap_loop(handle, 0, packet_handler, NULL);
  pcap_close(handle); // Close the handle when done.
}
