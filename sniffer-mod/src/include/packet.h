#ifndef PACKET_H
#define PACKET_H

enum PacketProtocol {
  TCP,
  UDP,
};

typedef struct {
  char *src_ip;
  int src_port;
  char *dst_ip;
  int dst_port;
  enum PacketProtocol protocol;
} TcpPacket;

typedef struct {
  char *src_ip;
  int src_port;
  char *dst_ip;
  int dst_port;
  enum PacketProtocol protocol;
} UdpPacket;

typedef TcpPacket *tcpPtr;
typedef UdpPacket *udpPtr;

tcpPtr t_new(char *src_ip, int src_port, char *dst_ip, int dst_port);
void t_free(tcpPtr self);

#endif
