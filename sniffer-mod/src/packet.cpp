#include "include/packet.h"
#include "include/flow.h"

#ifdef _WIN32
#include <winsock2.h> // Include Winsock2 header
#include <ws2tcpip.h> // Include for inet_ntop

// Link with the Winsock library
#pragma comment(lib, "ws2_32.lib")

// Ethernet Header
typedef struct ether_header {
  unsigned char ether_dhost[6]; // destination
  unsigned char ether_shost[6]; // source
  unsigned short ether_type;
} ETHHEADER, *PETHHEADER;

// Define ETHERTYPE_IP for Windows
#define ETHERTYPE_IP 0x0800
#endif

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

// Check the IP of the local machine
std::string local_addr = local_ip_addr();

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
             uint32_t source_address, uint32_t destination_address) {
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

  uint32_t src_ip =
      ntohl(*(reinterpret_cast<const uint32_t *>(ip_header + 12)));
  uint32_t dst_ip =
      ntohl(*(reinterpret_cast<const uint32_t *>(ip_header + 16)));
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
         (ip_header->source_address >> 24) & 0xFF,
         (ip_header->source_address >> 16) & 0xFF,
         (ip_header->source_address >> 8) & 0xFF,
         ip_header->source_address & 0xFF);
  printf("Destination Address: [%d.%d.%d.%d]\n",
         (ip_header->destination_address >> 24) & 0xFF,
         (ip_header->destination_address >> 16) & 0xFF,
         (ip_header->destination_address >> 8) & 0xFF,
         ip_header->destination_address & 0xFF);
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
  try {
    struct ether_header *eth_header;
    eth_header = (struct ether_header *)packet;

    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
      printf("Not an IP packet. Skipping....\n\n");
      return;
    }

    // Find start of IP header
    ip_header = packet + ETHERNET_HEADER_LEN;
    ipPtr new_ip_header = handle_ip_header(ip_header, packet);

    if (!new_ip_header) {
      fprintf(stderr, "Failed to handle IP header.\n");
      return;
    }

    // Initialize variables before the switch statement
    ipPtr new_ip_header_udp = NULL;
    cmbPtr new_udp_header = NULL;
    ipPtr new_ip_header_tcp = NULL;
    cmbPtr new_tcp_header = NULL;

    switch (new_ip_header->protocol) {
    case IPPROTO_UDP:
      printf("======================== UDP ===========================\n");

      new_ip_header_udp = handle_ip_header(ip_header, packet);
      if (!new_ip_header_udp) {
        fprintf(stderr, "Failed to handle UDP IP header.\n");
        break;
      }
      new_udp_header = handle_udp_header(packet, new_ip_header_udp);
      if (!new_udp_header) {
        fprintf(stderr, "Failed to handle UDP header.\n");
        break;
      }

      flow_add_or_update(new_ip_header_udp->source_address,
                         new_ip_header_udp->destination_address,
                         new_udp_header->src_port, new_udp_header->dst_port,
                         new_ip_header_udp->total_length,
                         new_ip_header_udp->protocol, local_addr);

      cmb_free(new_udp_header);
      new_udp_header = NULL;
      new_ip_header_udp = NULL;
      break;

    case IPPROTO_TCP:
      printf("======================== TCP ===========================\n");

      new_ip_header_tcp = handle_ip_header(ip_header, packet);
      if (!new_ip_header_tcp) {
        fprintf(stderr, "Failed to handle TCP IP header.\n");
        break;
      }
      new_tcp_header = handle_tcp_header(packet, new_ip_header_tcp);
      if (!new_tcp_header) {
        fprintf(stderr, "Failed to handle TCP header.\n");
        break;
      }

      flow_add_or_update(new_ip_header_tcp->source_address,
                         new_ip_header_tcp->destination_address,
                         new_tcp_header->src_port, new_tcp_header->dst_port,
                         new_ip_header_tcp->total_length,
                         new_ip_header_tcp->protocol, local_addr);

      cmb_free(new_tcp_header);
      new_tcp_header = NULL;
      new_ip_header_tcp = NULL;
      break;

    default:
      fprintf(stderr, "Unknown protocol: %d\n", new_ip_header->protocol);
      break;
    }

    ip_free(new_ip_header);
    new_ip_header = NULL;
  } catch (const std::exception &e) {
    std::cerr << "Error handling packet: " << e.what() << "\n";
  } catch (...) {
    std::cerr << "Unknown error handling packet.\n";
  }
}
