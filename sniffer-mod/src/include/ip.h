#ifndef IP_H
#define IP_H

#ifdef __cplusplus

#include <cstring>
#include <iostream>

std::string local_ip_addr();
std::string ip_to_str(uint32_t ip);

extern "C" {
#endif

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
  uint8_t octets[4];
} Ipv4Addr;

typedef Ipv4Addr *ipv4Ptr;

ipv4Ptr ipv4_new(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
void ipv4_free(ipv4Ptr self);

#ifdef __cplusplus
}
#endif

#endif
