#include <stdint.h>
#include <stdint.h> // For standard types like uint32_t, uint16_t, uint8_t

typedef uint32_t u_int;   // Define u_int as uint32_t
typedef uint16_t u_short; // Define u_short as uint16_t
typedef uint8_t u_char;   // Define u_char as uint8_t

#ifndef PACKET_H
#define PACKET_H

#include "ip.h"
#include <pcap.h>
#include <sys/types.h>

#define ETHERNET_HEADER_LEN 14
#define UDP_HEADER_LEN 8

enum PacketProtocol {
  TCP,
  UDP,
};

typedef struct {
  uint8_t version;
  uint8_t ihl;
  uint8_t tos;
  uint16_t total_length;
  uint16_t identification;
  uint8_t flags;
  uint16_t fragment_offset;
  uint8_t ttl;
  uint8_t protocol;
  uint16_t checksum;
  ipv4Ptr source_address;
  ipv4Ptr destination_address;
  uint32_t options;
  uint8_t padding;
} IpHeader;

typedef IpHeader *ipPtr;

typedef struct {
  ipPtr ip_header;
  uint16_t src_port;
  uint16_t dst_port;
  uint8_t header_length;
} FullTcpPacket; /* TCP packet struct */

typedef struct {
  ipPtr ip_header;
  uint16_t src_port;
  uint16_t dst_port;
} FullUdpPacket; /* UDP packet struct */

typedef FullTcpPacket *tcpPtr; /* For better readibility */
typedef FullUdpPacket *udpPtr; /* For better readibility */

tcpPtr
tcp_new(ipPtr ip_header, uint16_t src_port, uint16_t dst_port,
        uint8_t header_len); /* Create new TCP struct with assigned values */
ipPtr ip_new(uint8_t version, uint8_t ihl, uint8_t tos, uint16_t total_length,
             uint16_t identification, uint8_t flags, uint16_t fragment_offset,
             uint8_t ttl, uint8_t protocol, uint16_t checksum,
             ipv4Ptr source_address, ipv4Ptr destination_address,
             uint32_t options, uint8_t padding);
void tcp_free(tcpPtr self); /* Free the memory of struct and values */
void ip_free(ipPtr self);
ipPtr handle_ip_header(const u_char *ip_header,
                       const u_char *packet); /* Extracts info from
                                                 ip header */
tcpPtr handle_tcp_header(const u_char *packet, ipPtr ip_header);
void packet_handler(
    u_char *args, const struct pcap_pkthdr *header,
    const u_char *packet); /* Callback function for found packets */

#endif
