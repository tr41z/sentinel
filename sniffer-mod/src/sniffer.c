#include "include/sniffer.h"
#include <net/ethernet.h>
#include <stdio.h>
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
  /* Pointers to starting point of headers */
  const u_char *ip_header;
  const u_char *tcp_header;
  const u_char *payload;
  const u_char *udp_header;

  /* Header length in bytes */
  int ethernet_header_len = 14;
  int ip_header_len;
  int tcp_header_len;
  int payload_len;
  int udp_header_len;

  /* Find start of IP header */
  ip_header = packet + ethernet_header_len;
  /* The second-half of the first byte in ip_header
  contains the IP header length (IHL). */
  ip_header_len = ((*ip_header) & 0x0F);
  /* The IHL is number of 32-bit segments. Multiply
  by four to get a byte count for pointer arithmetic */
  ip_header_len = ip_header_len * 4;
  printf("IP header length (IHL) in bytes: %d\n", ip_header_len);

  u_char protocol = *(ip_header + 9);
  if (protocol != IPPROTO_TCP ||
      protocol != IPPROTO_UDP) { // refactor to catch actual UDP or TCP and
                                 // aggregate into corresponding `Packet` struct
    printf("Not a TCP or UDP Packet. Skipping...\n");
  }

  printf("-------------------------------------------------------\n");
  /* Refactor (put some stuff to functions) */
  switch (protocol) {
  case IPPROTO_UDP:
    udp_header = packet + ethernet_header_len + ip_header_len;
    udp_header_len = ((*(udp_header + 12)) & 0xF0) >> 4;
    udp_header_len = udp_header_len * 4;
    printf("UDP header length in bytes: %d\n", udp_header_len);

    int total_udp_header_size =
        ethernet_header_len + ip_header_len + udp_header_len;
    payload_len =
        header->caplen - (ethernet_header_len + ip_header_len + udp_header_len);
    printf("Payload size: %d bytes\n", payload_len);
    payload = packet + total_udp_header_size;
    printf("Memory address where payload begins: %p\n", payload);
    printf("-------------------------------------------------------\n");

    break;

  case IPPROTO_TCP:
    tcp_header = packet + ethernet_header_len + ip_header_len;
    tcp_header_len = ((*(tcp_header + 12)) & 0xF0) >> 4;
    tcp_header_len = tcp_header_len * 4;
    printf("TCP header length in bytes: %d\n", tcp_header_len);

    /* Add up all the header sizes to find payload */
    int total_tcp_header_size =
        ethernet_header_len + ip_header_len + tcp_header_len;
    payload_len =
        header->caplen - (ethernet_header_len + ip_header_len + tcp_header_len);
    printf("Payload size: %d bytes\n", payload_len);
    payload = packet + total_tcp_header_size;
    printf("Memory address where payload begins: %p\n", payload);
    printf("-------------------------------------------------------\n");
    break;
  }
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
