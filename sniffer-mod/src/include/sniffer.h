#ifndef SNIFFER_H
#define SNIFFER_H

#include <pcap/pcap.h>
#include <stdio.h>

typedef pcap_if_t *interPtr;

interPtr find_devices();
void start_sniffer(interPtr dev);
void packet_handler(u_char *args, const struct pcap_pkthdr *header,
                    const u_char *packet);

static inline void header_info(const struct pcap_pkthdr *header) {
  printf("------------------------------------\n");
  printf("PACKET LENGTH: [%i]\n", header->len);
  printf("------------------------------------\n");
}

#endif
