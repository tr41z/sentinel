#include "include/ip.h"

ipv4Ptr ipv4_new(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth) {
  // New ipv4 address initialisation
  ipv4Ptr ipv4_addr = (ipv4Ptr)calloc(1, sizeof(Ipv4Addr));

  // Assign new values
  ipv4_addr->octets[0] = first;
  ipv4_addr->octets[1] = second;
  ipv4_addr->octets[2] = third;
  ipv4_addr->octets[3] = fourth;

  return ipv4_addr;
}

// Better readibility between `ip_free`, `ipv4_free` & `cmb_free`
void ipv4_free(ipv4Ptr self) {
  if (self) {
    free(self);
  }
}

char *ip_to_str(ipv4Ptr ip) {
  static char ip_str[16]; // Enough space for "xxx.xxx.xxx.xxx\0"
  sprintf(ip_str, "%d.%d.%d.%d", ip->octets[0], ip->octets[1], ip->octets[2],
          ip->octets[3]);
  return ip_str;
}
