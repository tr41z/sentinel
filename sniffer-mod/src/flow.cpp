#include "include/flow.h"
#include "include/ip.h"
#include <string.h>

std::mutex flows_map_mutex; // Mutex for thread safety
FlowsMap flows_map;         // Global flows map

// Function to normalize the flow key
FlowKey create_normalized_key(ipv4Ptr src_ip, uint16_t src_port, ipv4Ptr dst_ip,
                              uint16_t dst_port, uint8_t protocol) {
  if (memcmp(src_ip, dst_ip, sizeof(Ipv4Addr)) < 0 ||
      (memcmp(src_ip, dst_ip, sizeof(Ipv4Addr)) == 0 && src_port < dst_port)) {
    return {src_ip, src_port, dst_ip, dst_port, protocol};
  } else {
    return {dst_ip, dst_port, src_ip, src_port, protocol};
  }
}

// Function to check and terminate flows based on thresholds
void terminate_and_save_flows() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::lock_guard<std::mutex> lock(flows_map_mutex);

    if (flows_map.empty()) {
      break;
    }

    for (auto it = flows_map.begin(); it != flows_map.end();) {
      auto idle_time = std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now() - it->second.last_update_time);
      auto working_time = std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now() - it->second.start_time);

      if (idle_time.count() >= IDLE_DURATION_MAX_THRESHOLD) {
        std::cout << "Terminating flow due to idle timeout: "
                  << ip_to_str(it->second.src_ip) << " -> "
                  << ip_to_str(it->second.dst_ip) << "\n";

        free(it->second.src_ip);
        free(it->second.dst_ip);
        it->second.src_ip = NULL;
        it->second.dst_ip = NULL;
        it = flows_map.erase(it);
      } else if (working_time.count() >= DURATION_MAX_THRESHOLD) {
        std::cout << "Terminating flow due to reaching max threshold: "
                  << ip_to_str(it->second.src_ip) << " -> "
                  << ip_to_str(it->second.dst_ip) << "\n";

        free(it->second.src_ip);
        free(it->second.dst_ip);
        it->second.src_ip = NULL;
        it->second.dst_ip = NULL;
        it = flows_map.erase(it);
      } else {
        ++it;
      }
    }

    // Print the number of flows left in the map after each round of termination
    std::cout << "Flows left: " << flows_map.size() << "\n";
  }
}

// Function to add or update a flow
void flow_add_or_update(ipv4Ptr src_ip, uint16_t src_port, ipv4Ptr dst_ip,
                        uint16_t dst_port, int total_bytes, uint8_t protocol) {
  std::lock_guard<std::mutex> lock(flows_map_mutex);

  // Check if the IPs involve the local machine
  char *local_addr = local_ip_addr();
  if ((strcmp(ip_to_str(src_ip), local_addr) != 0) &&
      (strcmp(ip_to_str(dst_ip), local_addr) != 0)) {
    printf("Skipping flow... (dst or src IP is not the local IP)\n");
    printf("Flow src_ip: %s\n", ip_to_str(src_ip));
    printf("Flow dst_ip: %s\n", ip_to_str(dst_ip));
    free(local_addr);
    return;
  }
  free(local_addr);

  // Normalize key
  FlowKey key =
      create_normalized_key(src_ip, src_port, dst_ip, dst_port, protocol);
  auto it = flows_map.find(key);

  if (it != flows_map.end()) {
    // Update existing flow
    it->second.total_bytes += total_bytes;
    it->second.packet_count += 1;
    it->second.last_update_time = std::chrono::system_clock::now();
    std::cout << "Flow Updated!\n";
  } else {
    // Create new flow with dynamically allocated IPs
    Flow flow;
    flow.src_ip = ipv4_new(src_ip->octets[0], src_ip->octets[1],
                           src_ip->octets[2], src_ip->octets[3]);
    flow.dst_ip = ipv4_new(dst_ip->octets[0], dst_ip->octets[1],
                           dst_ip->octets[2], dst_ip->octets[3]);
    flow.src_port = src_port;
    flow.dst_port = dst_port;
    flow.total_bytes = total_bytes;
    flow.protocol = protocol;
    flow.packet_count = 1;
    flow.start_time = std::chrono::system_clock::now();
    flow.last_update_time = std::chrono::system_clock::now();

    flows_map.insert({key, flow});
    std::cout << "Flow Created!\n";
  }

  // Print flow details
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
