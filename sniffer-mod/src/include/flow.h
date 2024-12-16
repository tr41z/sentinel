#ifndef FLOW_H
#define FLOW_H

#include "ip.h"
#include <arpa/inet.h>
#include <stdio.h>

#ifdef __cplusplus
#include <chrono>
#include <unordered_map>

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
    return src_ip == other.src_ip && src_port == other.src_port &&
           dst_ip == other.dst_ip && dst_port == other.dst_port &&
           protocol == other.protocol;
  }
};

struct FlowKeyHash {
  std::size_t operator()(const FlowKey &key) const {
    return std::hash<ipv4Ptr>()(key.src_ip) ^
           std::hash<uint16_t>()(key.src_port) ^
           std::hash<ipv4Ptr>()(key.dst_ip) ^
           std::hash<uint16_t>()(key.dst_port) ^
           std::hash<uint8_t>()(key.protocol);
  }
};

using FlowsMap = std::unordered_map<FlowKey, Flow, FlowKeyHash>;

extern FlowsMap flows_map;

FlowKey create_normalized_key(ipv4Ptr src_ip, uint16_t src_port, ipv4Ptr dst_ip,
                              uint16_t dst_port, uint8_t protocol);

extern "C" {
#endif

void flow_add_or_update(ipv4Ptr src_ip, uint16_t src_port, ipv4Ptr dst_ip,
                        uint16_t dst_port, int total_bytes, uint8_t protocol);

#ifdef __cplusplus
}
#endif

#endif
