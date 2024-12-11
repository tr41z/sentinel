#include "include/packet.h"
#include "include/ip.h"
#include <net/ethernet.h>
#include <stdint.h>
#include <stdio.h>
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

cmbPtr cmb_new(ipPtr ip_header, uint16_t src_port, uint16_t dst_port,
               uint8_t header_len) {
  // Allocating memory for new CMB packet
  cmbPtr cmb_packet = (cmbPtr)malloc(sizeof(CombinedPacket));

  // Case when memory wasn't allocated properly
  if (!cmb_packet) {
    fprintf(stderr, "Memory allocation failed for `cmb_packet`.\n");
    return NULL;
  }

  // Assign new values
  cmb_packet->ip_header = ip_header;
  cmb_packet->src_port = src_port;
  cmb_packet->dst_port = dst_port;
  cmb_packet->header_length = header_len;

  return cmb_packet; /* Return struct */
}

ipPtr ip_new(uint8_t version, uint8_t ihl, uint8_t tos, uint16_t total_length,
             uint16_t identification, uint8_t flags, uint16_t fragment_offset,
             uint8_t ttl, uint8_t protocol, uint16_t checksum,
             ipv4Ptr source_address, ipv4Ptr destination_address,
             uint32_t options, uint8_t padding) {
  ipPtr ip_header = (ipPtr)malloc(sizeof(IpHeader));

  if (!ip_header) {
    fprintf(stderr, "Memory allocation failed for `ip_header`.\n");
    return NULL;
  }

  ip_header->options = options;
  ip_header->padding = padding;
  ip_header->ttl = ttl;
  ip_header->source_address = source_address;
  ip_header->destination_address = destination_address;
  ip_header->ihl = ihl;
  ip_header->tos = tos;
  ip_header->flags = flags;
  ip_header->version = version;
  ip_header->checksum = checksum;
  ip_header->total_length = total_length;
  ip_header->identification = identification;
  ip_header->fragment_offset = fragment_offset;
  ip_header->protocol = protocol;

  return ip_header;
}

// Free the memory and destroy struct and pointer
void cmb_free(cmbPtr self) {
  if (self) {
    if (self->ip_header)
      ip_free(self->ip_header);
    free(self);
  }
}

void ip_free(ipPtr self) {
  if (self) {
    if (self->source_address)
      ipv4_free(self->source_address);
    if (self->destination_address)
      ipv4_free(self->destination_address);
    free(self);
  }
}

ipPtr handle_ip_header(const u_char *ip_header, const u_char *packet) {
  if (!ip_header) {
    fprintf(stderr, "Invalid IP header.\n");
    return NULL;
  }

  if (!packet) {
    fprintf(stderr, "Invalid packet. \n");
    return NULL;
  }

  ipv4Ptr src_ip = ipv4_new(*(ip_header + 12), *(ip_header + 13),
                            *(ip_header + 14), *(ip_header + 15));
  ipv4Ptr dst_ip = ipv4_new(*(ip_header + 16), *(ip_header + 17),
                            *(ip_header + 18), *(ip_header + 19));
  uint8_t ttl = *(ip_header + 8);
  uint8_t protocol = *(ip_header + 9);
  uint16_t checksum = ntohs((*(ip_header + 10) << 8) + *(ip_header + 11));
  uint8_t ihl = (*(ip_header) & 0x0F);
  ihl *= 4;

  /* Extract other features */

  // New IP header
  ipPtr new_ip_header = ip_new(100, ihl, 40, 2324, 1, 3, 88, ttl, protocol,
                               checksum, src_ip, dst_ip, 32, 10);
  return new_ip_header;
}

cmbPtr handle_tcp_header(const u_char *packet, ipPtr ip_header) {
  if (!ip_header) {
    fprintf(stderr, "Invalid IP header.\n");
    return NULL;
  }

  if (!packet) {
    fprintf(stderr, "Invalid packet. \n");
    return NULL;
  }

  ip_header_len = ip_header->ihl;
  tcp_header = packet + ETHERNET_HEADER_LEN + ip_header_len;

  uint16_t src_port = ntohs(*(uint16_t *)(tcp_header));
  uint16_t dst_port = ntohs(*(uint16_t *)(tcp_header + 2));
  uint8_t header_len = (*(tcp_header + 12) & 0xF0) >> 4;

  cmbPtr new_cmb_packet = cmb_new(ip_header, src_port, dst_port, header_len);
  return new_cmb_packet;
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
  ipPtr new_ip_header = handle_ip_header(ip_header, packet);

  switch (new_ip_header->protocol) {
  case IPPROTO_UDP:
    printf("======================== UDP ===========================\n");
    printf("========================================================\n\n");

    // cmbPtr new_udp_header = handle_udp_header(packet, new_ip_header);

    // prints

    // udp_free(new_udp_header);
    break;

  case IPPROTO_TCP:
    printf("======================== TCP ===========================\n");

    cmbPtr new_tcp_header = handle_tcp_header(packet, new_ip_header);

    printf("Source Port: [%d]\n", new_tcp_header->src_port);
    printf("Destination Port: [%d]\n", new_tcp_header->dst_port);
    printf("IHL: [%d]\n", new_ip_header->ihl);
    printf("========================================================\n\n");

    cmb_free(new_tcp_header);
    break;
  }

  ip_free(new_ip_header);
}
