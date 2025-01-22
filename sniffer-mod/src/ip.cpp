#include "include/ip.h"
#include <string>
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

std::string ip_to_str(uint32_t ip) {
  uint8_t byte1 = (ip >> 24) & 0xFF;
  uint8_t byte2 = (ip >> 16) & 0xFF;
  uint8_t byte3 = (ip >> 8) & 0xFF;
  uint8_t byte4 = ip & 0xFF;

  return std::to_string(byte1) + "." + std::to_string(byte2) + "." +
         std::to_string(byte3) + "." + std::to_string(byte4);
}

std::string local_ip_addr() {
#ifdef _WIN32
  WSADATA wsaData;
  int wsaInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (wsaInit != 0) {
    std::cerr << "WSAStartup failed with error: " << wsaInit << std::endl;
    return "";
  }
#endif

  const char *google_dns_server = "8.8.8.8";
  int dns_port = 53;

  struct sockaddr_in serv;
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  if (sock < 0) {
    std::cerr << "Socket creation error: " << strerror(errno) << std::endl;
#ifdef _WIN32
    WSACleanup();
#endif
    return "";
  }

  memset(&serv, 0, sizeof(serv));
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = inet_addr(google_dns_server);
  serv.sin_port = htons(dns_port);

  int err = connect(sock, (const struct sockaddr *)&serv, sizeof(serv));
  if (err < 0) {
    std::cerr << "Connection error: " << strerror(errno) << std::endl;
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
    return "";
  }

  struct sockaddr_in name;
  socklen_t namelen = sizeof(name);
  err = getsockname(sock, (struct sockaddr *)&name, &namelen);

  char buffer[80];
  const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, sizeof(buffer));
  if (p != NULL) {
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
    return std::string(buffer);
  } else {
    std::cerr << "inet_ntop error: " << strerror(errno) << std::endl;
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
    return "";
  }
}
