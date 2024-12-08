#ifndef IP_H
#define IP_H

#include <stdint.h>

typedef struct {
  uint8_t octets[];
} Ipv4Addr;

typedef Ipv4Addr *ipv4Ptr;

ipv4Ptr ipv4_new(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);

#endif
