#ifndef IP_H
#define IP_H

#ifdef __cplusplus
#include <cstring>
#include <iostream>
extern "C" {
#endif

// Platform-specific includes
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// IPv4 address structure
typedef struct {
  uint8_t octets[4];
} Ipv4Addr;

typedef Ipv4Addr *ipv4Ptr;

// Function declarations
ipv4Ptr ipv4_new(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
void ipv4_free(ipv4Ptr self);

#ifdef __cplusplus
}
std::string local_ip_addr();
std::string ip_to_str(uint32_t ip);
#endif

#endif /* IP_H */
