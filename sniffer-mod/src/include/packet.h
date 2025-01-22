#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include "ip.h"
#include <pcap.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ntddndis.h>
#else
#include <arpa/inet.h>
#include <net/ethernet.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define ETHERNET_HEADER_LEN 14
#define UDP_HEADER_LEN 8

typedef uint32_t u_int;   /* Define u_int as uint32_t */
typedef uint16_t u_short; /* Define u_short as uint16_t */
typedef uint8_t u_char;   /* Define u_char as uint8_t */

/* Struct for collecting information from IP Header. Skipping options and padding */
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
    uint32_t source_address;
    uint32_t destination_address;
} IpHeader;

typedef IpHeader *ipPtr;

/* Combined Packet struct: Combination of IP Header and shared features from TCP/UDP packets */
typedef struct {
    ipPtr ip_header;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t header_length;
} CombinedPacket;

typedef CombinedPacket *cmbPtr; /* For better readability */

/* Function declarations */
cmbPtr cmb_new(ipPtr ip_header, uint16_t src_port, uint16_t dst_port, uint8_t header_len);
ipPtr ip_new(uint8_t version, uint8_t ihl, uint8_t tos, uint16_t total_length,
             uint16_t identification, uint8_t flags, uint16_t fragment_offset,
             uint8_t ttl, uint8_t protocol, uint16_t checksum,
             uint32_t source_address, uint32_t destination_address);
void cmb_free(cmbPtr self); /* Free the memory of struct and values */
void ip_free(ipPtr self);
ipPtr handle_ip_header(const u_char *ip_header, const u_char *packet); /* Extracts info from IP header */
cmbPtr handle_tcp_header(const u_char *packet, ipPtr ip_header);
cmbPtr handle_udp_header(const u_char *packet, ipPtr ip_header);
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet); /* Callback for found packets */
void display_packet(ipPtr ip_header, cmbPtr proto_header);

#endif /* PACKET_H */
