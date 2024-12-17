#ifndef FLOW_H
#define FLOW_H

#include "ip.h"
#include <arpa/inet.h>
#include <stdio.h>

#ifdef __cplusplus
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>

#define IDLE_DURATION_MAX_THRESHOLD 5
#define DURATION_MAX_THRESHOLD 5

struct Flow {
  ipv4Ptr src_ip;
  uint16_t src_port;
  ipv4Ptr dst_ip;
  uint16_t dst_port;
  int total_bytes;
  uint8_t protocol;
  int packet_count = 1;
  std::chrono::system_clock::time_point start_time;
  std::chrono::system_clock::time_point last_update_time;
};

struct FlowKey {
  ipv4Ptr src_ip;
  uint16_t src_port;
  ipv4Ptr dst_ip;
  uint16_t dst_port;
  uint8_t protocol;

  bool operator==(const FlowKey &other) const {
    return std::memcmp(src_ip, other.src_ip, sizeof(Ipv4Addr)) == 0 &&
           src_port == other.src_port &&
           std::memcmp(dst_ip, other.dst_ip, sizeof(Ipv4Addr)) == 0 &&
           dst_port == other.dst_port && protocol == other.protocol;
  }
};

struct FlowKeyHash {
  std::size_t operator()(const FlowKey &key) const {
    std::size_t h1 = std::hash<std::string>()(
        std::string((char *)key.src_ip, sizeof(Ipv4Addr)));
    std::size_t h2 = std::hash<std::string>()(
        std::string((char *)key.dst_ip, sizeof(Ipv4Addr)));
    return h1 ^ (h2 << 1) ^ std::hash<uint16_t>()(key.src_port) ^
           std::hash<uint16_t>()(key.dst_port) ^
           std::hash<uint8_t>()(key.protocol);
  }
};

using FlowsMap = std::unordered_map<FlowKey, Flow, FlowKeyHash>;

extern FlowsMap flows_map;

FlowKey create_normalized_key(ipv4Ptr src_ip, uint16_t src_port, ipv4Ptr dst_ip,
                              uint16_t dst_port,
                              uint8_t protocol); /* Normalizing key */
void terminate_and_save_flows(); /* Async function to terminate flows based on
                                  * their duration */

extern "C" {
#endif

void flow_add_or_update(
    ipv4Ptr src_ip, uint16_t src_port, ipv4Ptr dst_ip, uint16_t dst_port,
    int total_bytes,
    uint8_t protocol); /* Function that either adds or updates existing flow
                          based on its existance in map */

#ifdef __cplusplus
}
#endif

#endif
