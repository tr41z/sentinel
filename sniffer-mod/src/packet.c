#include "include/packet.h"
#include <stdlib.h>
#include <string.h>

tcpPtr t_new(char *src_ip, int src_port, char *dst_ip, int dst_port) {
  tcpPtr tcpPacket = (tcpPtr)malloc(sizeof(TcpPacket));

  if (!tcpPacket) {
    return NULL;
  }

  tcpPacket->src_ip = strdup(src_ip);
  tcpPacket->src_port = src_port;
  tcpPacket->dst_ip = strdup(dst_ip);
  tcpPacket->dst_port = dst_port;
  tcpPacket->protocol = TCP;
  return tcpPacket;
}

void t_free(tcpPtr self) {
  if (self) {
    free(self->src_ip);
    free(self->dst_ip);
    free(self);
    self = NULL;
  }
}
