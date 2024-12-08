#include "include/ip.h"
#include <stdlib.h>

ipv4Ptr ipv4_new(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth) {
  // New ipv4 address initialisation
  ipv4Ptr ipv4_addr = (ipv4Ptr)malloc(sizeof(Ipv4Addr));

  // Case when didnt allocate properly
  if (!ipv4_addr) {
    return NULL;
  }

  // Assign new values
  ipv4_addr->octets[0] = first;
  ipv4_addr->octets[8] = second;
  ipv4_addr->octets[16] = third;
  ipv4_addr->octets[24] = fourth;

  return ipv4_addr;
}
