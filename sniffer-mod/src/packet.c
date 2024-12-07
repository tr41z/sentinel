#include "include/packet.h"
#include <stdlib.h>

tcpPtr t_new(char *src_ip, int src_port, char *dst_ip, int dst_port) {
  tcpPtr tcpPacket = (tcpPtr)malloc(sizeof(TcpPacket));
  tcpPacket->src_ip = src_ip;
  tcpPacket->src_port = src_port;
  tcpPacket->dst_ip = dst_ip;
  tcpPacket->dst_port = dst_port;
  tcpPacket->protocol = TCP;
  return tcpPacket;
}

void t_free(tcpPtr self) {
  if (self) {
    free(self->src_ip);
    free(self->dst_ip);
    free(self);
  }
}
