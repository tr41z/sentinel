#ifndef IP_H
#define IP_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
  uint8_t octets[4];
} Ipv4Addr;

typedef Ipv4Addr *ipv4Ptr;

ipv4Ptr ipv4_new(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
void ipv4_free(ipv4Ptr self);

#endif