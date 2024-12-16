#include "include/flow.h"
#include "include/ip.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdio.h>

// Global map instance
FlowsMap flows_map;

FlowKey create_normalized_key(ipv4Ptr src_ip, uint16_t src_port, ipv4Ptr dst_ip,
                              uint16_t dst_port, uint8_t protocol) {
  if (src_ip < dst_ip || (src_ip == dst_ip && src_port < dst_port)) {
    return {src_ip, src_port, dst_ip, dst_port, protocol};
  } else {
    return {dst_ip, dst_port, src_ip, src_port, protocol};
  }
}

void flow_add_or_update(ipv4Ptr src_ip, uint16_t src_port, ipv4Ptr dst_ip,
                        uint16_t dst_port, int total_bytes, uint8_t protocol) {
  FlowKey key =
      create_normalized_key(src_ip, src_port, dst_ip, dst_port, protocol);
  auto it = flows_map.find(key);

  if (it != flows_map.end()) {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
        it->second.last_update_time - it->second.start_time);
    auto dur = seconds.count();

    if (dur >= IDLE_DURATION_MAX_THRESHOLD) {
      std::cout << "!!!! TERMINATING FLOW !!!!" << std::endl;
      flows_map.erase(it);
    } else {
      // Update existing flow data
      it->second.total_bytes += total_bytes;
      it->second.packet_count += 1;
      it->second.last_update_time = std::chrono::system_clock::now();
      std::cout << "Flow Updated!\n";
    }
  } else {
    // If flow doesn't exist, create new
    std::cout << "Flow Created!\n";
    Flow flow;
    flow.src_ip = src_ip;
    flow.src_port = src_port;
    flow.dst_ip = dst_ip;
    flow.dst_port = dst_port;
    flow.total_bytes = total_bytes;
    flow.protocol = protocol;
    flow.start_time = std::chrono::system_clock::now();
    flow.last_update_time = std::chrono::system_clock::now();
    flows_map.insert(std::make_pair(key, flow));
  }

  const Flow &value = it != flows_map.end() ? it->second : flows_map[key];
  auto start_time = std::chrono::system_clock::to_time_t(value.start_time);
  auto last_update_time =
      std::chrono::system_clock::to_time_t(value.last_update_time);
  std::cout << "src_ip: " << ip_to_str(value.src_ip) << "\n"
            << "src_port: " << value.src_port << "\n"
            << "dst_ip: " << ip_to_str(value.dst_ip) << "\n"
            << "dst_port: " << value.dst_port << "\n"
            << "protocol: " << static_cast<int>(value.protocol) << "\n"
            << "total_bytes: " << value.total_bytes << "\n"
            << "packet_count: " << value.packet_count << "\n"
            << "start_time: "
            << std::put_time(std::localtime(&start_time), "%Y-%m-%d %H:%M:%S")
            << "\n"
            << "last_update_time: "
            << std::put_time(std::localtime(&last_update_time),
                             "%Y-%m-%d %H:%M:%S")
            << "\n";
}
