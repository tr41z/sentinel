#include "include/ip.h"
#include <string>

std::string ip_to_str(uint32_t ip) {
  // Extract individual bytes from the uint32_t IP address
  uint8_t byte1 = (ip >> 24) & 0xFF;
  uint8_t byte2 = (ip >> 16) & 0xFF;
  uint8_t byte3 = (ip >> 8) & 0xFF;
  uint8_t byte4 = ip & 0xFF;

  // Format the IP address as a string in dotted-decimal format
  return std::to_string(byte1) + "." + std::to_string(byte2) + "." +
         std::to_string(byte3) + "." + std::to_string(byte4);
}

std::string local_ip_addr() {
  const char *google_dns_server = "8.8.8.8";
  int dns_port = 53;

  struct sockaddr_in serv;
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  // Socket could not be created
  if (sock < 0) {
    std::cout << "Socket error" << std::endl;
    return "";
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
    return "";
  }

  struct sockaddr_in name;
  socklen_t namelen = sizeof(name);
  err = getsockname(sock, (struct sockaddr *)&name, &namelen);

  char buffer[80]; // Use a fixed-size buffer instead of dynamic allocation
  const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, sizeof(buffer));
  if (p != NULL) {
    std::cout << "Local IP address is: " << buffer << std::endl;
    close(sock);
    return std::string(buffer); // Return as std::string
  } else {
    std::cout << "Error number: " << errno
              << ". Error message: " << strerror(errno) << std::endl;
    close(sock);
    return "";
  }
}
