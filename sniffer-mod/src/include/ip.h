#ifndef IP_H
#define IP_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t octets[4];
} Ipv4Addr;

typedef Ipv4Addr *ipv4Ptr;

ipv4Ptr ipv4_new(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
void ipv4_free(ipv4Ptr self);
char *ip_to_str(ipv4Ptr ip);

#ifdef __cplusplus
}
#endif

#endif
