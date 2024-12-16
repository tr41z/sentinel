#include "include/flow.h"
#include <iomanip>
#include <iostream>

// Global map instance
FlowsMap flows_map;

// Need to implement bidirectional packet -> flow aggregation
void add_or_update(ipv4Ptr src_ip, uint16_t src_port, ipv4Ptr dst_ip,
                   uint16_t dst_port, int total_bytes, uint8_t protocol) {
  FlowKey key = {src_ip, src_port, dst_ip, dst_port, protocol};
  auto it = flows_map.find(key);

  if (it != flows_map.end()) {
    it->second.total_bytes += total_bytes;
    it->second.last_update_time = std::chrono::system_clock::now();
  } else {
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

  for (auto it = flows_map.begin(); it != flows_map.end(); ++it) {
    const FlowKey &key = it->first;
    const Flow &value = it->second;
    std::cout << "FlowKey -> src_ip: " << key.src_ip
              << ", src_port: " << key.src_port << ", dst_ip: " << key.dst_ip
              << ", dst_port: " << key.dst_port
              << ", protocol: " << static_cast<int>(key.protocol) << "\n";
    auto start_time = std::chrono::system_clock::to_time_t(value.start_time);
    auto last_update_time =
        std::chrono::system_clock::to_time_t(value.last_update_time);
    std::cout << "Flow -> total_bytes: " << value.total_bytes
              << ", start_time: "
              << std::put_time(std::localtime(&start_time), "%Y-%m-%d %H:%M:%S")
              << ", last_update_time: "
              << std::put_time(std::localtime(&last_update_time),
                               "%Y-%m-%d %H:%M:%S")
              << "\n";
  }
}
