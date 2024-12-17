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

char *local_ip_addr() {
  const char *google_dns_server = "8.8.8.8";
  int dns_port = 53;

  struct sockaddr_in serv;
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  // Socket could not be created
  if (sock < 0) {
    std::cout << "Socket error" << std::endl;
    return nullptr;
  }

  memset(&serv, 0, sizeof(serv));
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = inet_addr(google_dns_server);
  serv.sin_port = htons(dns_port);

  int err = connect(sock, (const struct sockaddr *)&serv, sizeof(serv));
  if (err < 0) {
    std::cout << "Error number: " << errno
              << ". Error message: " << strerror(errno) << std::endl;
    close(sock);
    return nullptr;
  }

  struct sockaddr_in name;
  socklen_t namelen = sizeof(name);
  err = getsockname(sock, (struct sockaddr *)&name, &namelen);

  char *buffer = new char[80]; /* Dynamically allocate memory for buffer */
  const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
  if (p != NULL) {
    std::cout << "Local IP address is: " << buffer << std::endl;
  } else {
    std::cout << "Error number: " << errno
              << ". Error message: " << strerror(errno) << std::endl;
    delete[] buffer; /* Clean up memory */
    close(sock);
    return nullptr;
  }

  close(sock);   /* Close the socket before returning */
  return buffer; /* Return the dynamically allocated buffer */
}
