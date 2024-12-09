#ifndef PACKET_H
#define PACKET_H

#include "ip.h"
#include <pcap/pcap.h>

#define ETHERNET_HEADER_LEN 14
#define UDP_HEADER_LEN 8

enum PacketProtocol {
  TCP,
  UDP,
};

typedef struct {
  ipv4Ptr src_ip;
  int src_port;
  ipv4Ptr dst_ip;
  int dst_port;
  enum PacketProtocol protocol;
} TcpPacket; /* TCP packet struct */

typedef struct {
  ipv4Ptr src_ip;
  int src_port;
  ipv4Ptr dst_ip;
  int dst_port;
  enum PacketProtocol protocol;
} UdpPacket; /* UDP packet struct */

typedef TcpPacket *tcpPtr; /* For better readibility */
typedef UdpPacket *udpPtr; /* For better readibility */

tcpPtr tcp_new(ipv4Ptr src_ip, int src_port, ipv4Ptr dst_ip,
               int dst_port); /* Create new TCP struct with assigned values */
void tcp_free(tcpPtr self);   /* Free the memory of struct and values */
void handle_ip_header(const u_char *ip_header,
                      const u_char *packet); /* Extracts info from ip header */
void packet_handler(
    u_char *args, const struct pcap_pkthdr *header,
    const u_char *packet); /* Callback function for found packets */

#endif
