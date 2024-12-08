#include "include/packet.h"
#include "include/ip.h"
#include <stdlib.h>
#include <string.h>

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
