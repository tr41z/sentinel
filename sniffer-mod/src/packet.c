#include "include/packet.h"
#include "include/flow.h"
#include "include/ip.h"

struct pcap_pkthdr header;       /* The header that pcap returns */
struct ether_header *eth_header; /* Starting point of ethernet header */

const u_char *packet;     /* Raw packet */
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
             ipv4Ptr source_address, ipv4Ptr destination_address) {
  ipPtr ip_header = (ipPtr)malloc(sizeof(IpHeader));

  if (!ip_header) {
    fprintf(stderr, "Memory allocation failed for `ip_header`.\n");
    return NULL;
  }

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
  uint16_t checksum = ntohs(*(uint16_t *)(ip_header + 10));
  uint8_t ihl = ((*(ip_header) & 0x0F) * 4);
  uint8_t version = ((*(ip_header) & 0xF0) >> 4);
  uint8_t tos = *(ip_header + 1);
  uint16_t total_length = ntohs(*(uint16_t *)(ip_header + 2));
  uint16_t identification = ntohs(*(uint16_t *)(ip_header + 4));
  uint8_t flags = ((*(ip_header + 6) & 0xE0) >> 5);
  uint16_t fragment_offset =
      ((*(ip_header + 6) & 0x1F) << 8) | *(ip_header + 7);

  // New IP header
  ipPtr new_ip_header =
      ip_new(version, ihl, tos, total_length, identification, flags,
             fragment_offset, ttl, protocol, checksum, src_ip, dst_ip);
  return new_ip_header;
}

cmbPtr handle_tcp_header(const u_char *packet, ipPtr ip_header) {
  if (!ip_header) {
    fprintf(stderr, "Invalid IP header.\n");
    return NULL;
  }

  if (!packet) {
    fprintf(stderr, "Invalid packet.\n");
    return NULL;
  }

  ip_header_len = ip_header->ihl;
  tcp_header = packet + ETHERNET_HEADER_LEN + ip_header_len;

  uint16_t src_port = ntohs(*(uint16_t *)(tcp_header));
  uint16_t dst_port = ntohs(*(uint16_t *)(tcp_header + 2));
  uint8_t header_len = ((*(tcp_header + 12) >> 4) * 4);

  cmbPtr new_cmb_packet = cmb_new(ip_header, src_port, dst_port, header_len);
  return new_cmb_packet;
}

cmbPtr handle_udp_header(const u_char *packet, ipPtr ip_header) {
  if (!ip_header) {
    fprintf(stderr, "Invalid IP header.\n");
    return NULL;
  }

  if (!packet) {
    fprintf(stderr, "Invalid packet.\n");
    return NULL;
  }

  ip_header_len = ip_header->ihl;
  udp_header = packet + ETHERNET_HEADER_LEN + ip_header_len;

  uint16_t src_port = ntohs(*(uint16_t *)(udp_header));
  uint16_t dst_port = ntohs(*(uint16_t *)(udp_header + 2));

  cmbPtr new_cmb_packet =
      cmb_new(ip_header, src_port, dst_port, UDP_HEADER_LEN);
  return new_cmb_packet;
}

void display_packet(ipPtr ip_header, cmbPtr proto_header) {
  printf("Version: [%d]\n", ip_header->version);
  printf("Source Address: [%d.%d.%d.%d]\n",
         ip_header->source_address->octets[0],
         ip_header->source_address->octets[1],
         ip_header->source_address->octets[2],
         ip_header->source_address->octets[3]);
  printf("Destination Address: [%d.%d.%d.%d]\n",
         ip_header->destination_address->octets[0],
         ip_header->destination_address->octets[1],
         ip_header->destination_address->octets[2],
         ip_header->destination_address->octets[3]);
  printf("Source Port: [%d]\n", proto_header->src_port);
  printf("Destination Port: [%d]\n", proto_header->dst_port);
  printf("IHL: %d bytes\n", ip_header->ihl);
  printf("TOS: [%d]\n", ip_header->tos);
  printf("IP Total Length: %d bytes\n", ip_header->total_length);
  printf("Checksum: [%d]\n", ip_header->checksum);
  printf("Identification: [%d]\n", ip_header->identification);
  printf("Flags: [%d]\n", ip_header->flags);
  printf("Fragment Offset: [%d]\n", ip_header->fragment_offset);
  printf("Header Length: %d bytes\n", proto_header->header_length);
  printf("========================================================\n\n");
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

    ipPtr new_ip_header_udp = handle_ip_header(ip_header, packet);
    cmbPtr new_udp_header = handle_udp_header(packet, new_ip_header_udp);

    flow_add_or_update(
        new_ip_header_udp->source_address, new_udp_header->src_port,
        new_ip_header_udp->destination_address, new_udp_header->dst_port,
        new_ip_header_udp->total_length, new_ip_header_udp->protocol);

    cmb_free(new_udp_header);
    new_udp_header = NULL;
    new_ip_header_udp = NULL;
    break;

  case IPPROTO_TCP:
    printf("======================== TCP ===========================\n");

    ipPtr new_ip_header_tcp = handle_ip_header(ip_header, packet);
    cmbPtr new_tcp_header = handle_tcp_header(packet, new_ip_header_tcp);

    flow_add_or_update(
        new_ip_header_tcp->source_address, new_tcp_header->src_port,
        new_ip_header_tcp->destination_address, new_tcp_header->dst_port,
        new_ip_header_tcp->total_length, new_ip_header_tcp->protocol);

    cmb_free(new_tcp_header);
    new_tcp_header = NULL;
    new_ip_header_tcp = NULL;
    break;
  }

  ip_free(new_ip_header);
  new_ip_header = NULL;
}
