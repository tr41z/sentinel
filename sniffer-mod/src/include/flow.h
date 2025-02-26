#ifndef FLOW_H
#define FLOW_H

#ifdef __cplusplus

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include "sqlite3.h"
#include <thread>
#include <unordered_map>
#include <unordered_set>

#define IDLE_DURATION_MAX_THRESHOLD 30
#define DURATION_MAX_THRESHOLD 180
#define REFRESH_RATE 1

struct Flow {
  uint32_t src_ip;
  uint32_t dst_ip;
  int total_bytes;
  uint8_t protocol;
  double rate;
  double avg_packet_size;
  int packet_count = 1;
  std::unordered_set<uint16_t> src_ports;
  std::unordered_set<uint16_t> dst_ports;
  std::chrono::system_clock::time_point start_time;
  std::chrono::system_clock::time_point last_update_time;
  std::chrono::seconds duration;
};

struct FlowKey {
  uint32_t src_ip;
  uint32_t dst_ip;
  uint8_t protocol;

  // Comparison operator for FlowKey
  bool operator==(const FlowKey &other) const {
    return src_ip == other.src_ip && dst_ip == other.dst_ip &&
           protocol == other.protocol;
  }

  // Comparison operator for FlowKey (Inequality)
  bool operator!=(const FlowKey &other) const {
    return !(*this == other); // calls the operator== and negates the result
  }
};

// Custom hash function for FlowKey
struct FlowKeyHash {
  std::size_t operator()(const FlowKey &key) const {
    std::size_t h1 = std::hash<uint32_t>()(key.src_ip);
    std::size_t h2 = std::hash<uint32_t>()(key.dst_ip);
    return h1 ^ (h2 << 1) ^ std::hash<uint8_t>()(key.protocol);
  }
};

using FlowsMap = std::unordered_map<FlowKey, Flow, FlowKeyHash>;

extern FlowsMap flows_map;

// Function to create a normalized flow key
FlowKey create_normalized_key(uint32_t src_ip, uint32_t dst_ip,
                              uint8_t protocol);

// Async function to terminate and save flows based on their duration
void terminate_and_save_flows(sqlite3 *db);

extern "C" {
#endif

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include <stdio.h>

// Function to add or update flows in the map
void flow_add_or_update(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port,
                        uint16_t dst_port, int total_bytes, uint8_t protocol,
                        std::string local_addr);

#ifdef __cplusplus
}
#endif

#endif
