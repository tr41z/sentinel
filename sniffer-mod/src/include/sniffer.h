#ifndef SNIFFER_H
#define SNIFFER_H

#define ETHERNET_HEADER_LEN 14
#define UDP_HEADER_LEN 8

#include <pcap/pcap.h>
#include <stdio.h>

typedef pcap_if_t *interPtr; /* For better readibility */

interPtr find_devices();          /* Returns interfaces to sniff on */
void start_sniffer(interPtr dev); /* Starts sniffer on device (interface) */
void handle_ip_header(const u_char *ip_header,
                      const u_char *packet); /* Extracts info from ip header */
void packet_handler(
    u_char *args, const struct pcap_pkthdr *header,
    const u_char *packet); /* Callback function for found packets */

#endif
