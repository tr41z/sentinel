#ifndef SNIFFER_H
#define SNIFFER_H

#include <pcap/pcap.h>
#include <stdio.h>

typedef pcap_if_t *interPtr;

interPtr find_devices();
void start_sniffer(interPtr dev);
static inline void header_info(struct pcap_pkthdr header) {
  printf("------------------------------------\n");
  printf("PACKET LENGTH: [%i]\n", header.len);
  printf("------------------------------------\n");
}

#endif
