#include "include/packet.h"
#include <net/ethernet.h>
#include <stdlib.h>
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

// Strings for development only
char src_ip_str[20];
char dst_ip_str[20];

tcpPtr tcp_new(ipv4Ptr src_ip, int src_port, ipv4Ptr dst_ip, int dst_port) {
  // Allocating memory for new TCP packet
  tcpPtr tcp_packet = (tcpPtr)malloc(sizeof(TcpPacket));

  // Case when memory wasn't allocated properly
  if (!tcp_packet)
    return NULL;

  // Assign new values
  tcp_packet->src_ip = src_ip;
  tcp_packet->src_port = src_port;
  tcp_packet->dst_ip = dst_ip;
  tcp_packet->dst_port = dst_port;
  tcp_packet->protocol = TCP;

  return tcp_packet; /* Return struct */
}

// Free the memory and destroy struct and pointer
void tcp_free(tcpPtr self) {
  if (self) {
    if (self->src_ip)
      ipv4_free(self->src_ip);
    if (self->dst_ip)
      ipv4_free(self->dst_ip);
    free(self);
  }
}

void handle_ip_header(const u_char *ip_header, const u_char *packet) {
  ipv4Ptr src_ip = ipv4_new(*(ip_header + 12), *(ip_header + 13),
                            *(ip_header + 14), *(ip_header + 15));
  ipv4Ptr dst_ip = ipv4_new(*(ip_header + 16), *(ip_header + 17),
                            *(ip_header + 18), *(ip_header + 19));

  // For testning purposes only
  sprintf(src_ip_str, "%d.%d.%d.%d", src_ip->octets[0], src_ip->octets[1],
          src_ip->octets[2], src_ip->octets[3]);
  sprintf(dst_ip_str, "%d.%d.%d.%d", dst_ip->octets[0], dst_ip->octets[2],
          dst_ip->octets[2], dst_ip->octets[3]);

  printf("Source IP Address: [%s]\n", src_ip_str);
  printf("Destination IP Address: [%s]\n", dst_ip_str);
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

  switch (protocol) {
  case IPPROTO_UDP:
    printf("======================== UDP ===========================\n");
    handle_ip_header(ip_header, packet);
    udp_header = packet + ETHERNET_HEADER_LEN + ip_header_len;
    printf("========================================================\n");

    break;

  case IPPROTO_TCP:
    printf("======================== TCP ===========================\n");
    handle_ip_header(ip_header, packet);
    tcp_header = packet + ETHERNET_HEADER_LEN + ip_header_len;
    tcp_header_len = (*(tcp_header + 12) & 0xF0) >> 4;

    // Add up all the header sizes to find payload
    int total_tcp_header_size =
        ETHERNET_HEADER_LEN + ip_header_len + tcp_header_len;
    payload_len =
        header->caplen - (ETHERNET_HEADER_LEN + ip_header_len + tcp_header_len);
    printf("Payload size: %d bytes\n", payload_len);
    payload = packet + total_tcp_header_size;
    printf("Memory address where payload begins: %p\n", payload);
    printf("========================================================\n");

    break;
  }
}
