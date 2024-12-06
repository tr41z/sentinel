#ifndef PACKET_H
#define PACKET_H

enum PacketProtocol {
  TCP,
  UDP,
};

struct TcpPacket {
  PacketProtocol protocol;
};
struct UdpPacket {
  PacketProtocol protocol;
};

typedef TcpPacket *tcpPtr;
typedef UdpPacket *udpPtr;

#endif
