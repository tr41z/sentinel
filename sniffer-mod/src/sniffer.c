#include "include/sniffer.h"
#include "include/ip.h"
#include <net/ethernet.h>
#include <stdio.h>
#include <string.h>

struct pcap_pkthdr header;       /* The header that pcap returns */
struct ether_header *eth_header; /* Starting point of ethernet header */

const u_char *packet;     /* raw packet */
const u_char *ip_header;  /* Starting point of ip header */
const u_char *tcp_header; /* Starting point of tcp header */
const u_char *udp_header; /* Starting point of udp header */
const u_char *payload;    /* Starting point of payload in header */

// Header length in bytes
int ip_header_len;  /* Initialisation of ip header len since its not fixed */
int tcp_header_len; /* Initialisaion of tcp header len since its not fixed */
int payload_len;    /* Initialisaion of payload len since its not fixed */

char err_buff[PCAP_ERRBUF_SIZE]; /* Error string */
pcap_t *handle;                  /* Session handle */
pcap_if_t *interface, *temp;     /* Interfaces */

// Strings for development only
char src_ip_str[20];

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
      return temp; /* Return en0 if found */
    }
  }

  // If en0 is not found
  printf("Device en0 not found!\n");
  return NULL;
}

void handle_ip_header(const u_char *ip_header, const u_char *packet) {
  ipv4Ptr src_ip = ipv4_new(*(ip_header + 12), *(ip_header + 13),
                            *(ip_header + 14), *(ip_header + 15));

  sprintf(src_ip_str, "%d.%d.%d.%d", src_ip->octets[0], src_ip->octets[1],
          src_ip->octets[2], src_ip->octets[3]);

  printf("Source IP Address: [%s]\n", src_ip_str);
}

void packet_handler(u_char *args, const struct pcap_pkthdr *header,
                    const u_char *packet) {
  eth_header = (struct ether_header *)packet;
  if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
    printf("Not an IP packet. Skipping....\n\n");
    return;
  }

  // Find start of IP header
  ip_header = packet + ETHERNET_HEADER_LEN;

  u_char protocol = *(ip_header + 9);
  if (protocol != IPPROTO_TCP && protocol != IPPROTO_UDP) {
    printf("Not a TCP or UDP Packet. Skipping...\n");
  }

  printf("-------------------------------------------------------\n");
  handle_ip_header(ip_header, packet);

  switch (protocol) {
  case IPPROTO_UDP:
    udp_header = packet + ETHERNET_HEADER_LEN + ip_header_len;

    break;

  case IPPROTO_TCP:
    tcp_header = packet + ETHERNET_HEADER_LEN + ip_header_len;
    tcp_header_len = ((*(tcp_header + 12)) & 0xF0) >> 4;
    tcp_header_len = tcp_header_len * 4;

    // Add up all the header sizes to find payload
    int total_tcp_header_size =
        ETHERNET_HEADER_LEN + ip_header_len + tcp_header_len;
    payload_len =
        header->caplen - (ETHERNET_HEADER_LEN + ip_header_len + tcp_header_len);
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
  pcap_close(handle); /* Close the handle when done */
}
