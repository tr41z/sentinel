#include "include/flow.h"
#include "include/db.h"
#include "include/ip.h"
#include <chrono>
#include <string>

std::mutex flows_map_mutex; // Mutex for thread safety
FlowsMap flows_map;         // Global flows map

FlowKey create_normalized_key(uint32_t src_ip, uint32_t dst_ip,
                              uint8_t protocol) {
  if (src_ip < dst_ip) {
    return {src_ip, dst_ip, protocol};
  } else {
    return {dst_ip, src_ip, protocol};
  }
}

// Function to check and terminate flows based on thresholds
void terminate_and_save_flows(sqlite3 *db) {
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(REFRESH_RATE));
    std::lock_guard<std::mutex> lock(flows_map_mutex);

    for (auto it = flows_map.begin(); it != flows_map.end();) {
      try {
        auto idle_time = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now() - it->second.last_update_time);
        auto working_time = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now() - it->second.start_time);

        if (idle_time.count() >= IDLE_DURATION_MAX_THRESHOLD) {
          std::cout << "Terminating flow due to idle timeout: "
                    << ip_to_str(it->second.src_ip) << " -> "
                    << ip_to_str(it->second.dst_ip) << "\n";

          save_flow(db, it->second);
          if (db == nullptr) {
            std::cerr << "Error: Database connection lost while saving flow." << std::endl;
            return;
          }

          it = flows_map.erase(it);
        } else if (working_time.count() >= DURATION_MAX_THRESHOLD) {
          std::cout << "Terminating flow due to reaching max threshold: "
                    << ip_to_str(it->second.src_ip) << " -> "
                    << ip_to_str(it->second.dst_ip) << "\n";

          save_flow(db, it->second);
          if (db == nullptr) {
            std::cerr << "Error: Database connection lost while saving flow." << std::endl;
            return;
          }

          it = flows_map.erase(it);
        } else {
          ++it;
        }
      } catch (const std::exception &e) {
        std::cerr << "Error terminating flow: " << e.what() << "\n";
        ++it; // Move to the next flow to avoid infinite loop
      } catch (...) {
        std::cerr << "Unknown error terminating flow.\n";
        ++it; // Move to the next flow to avoid infinite loop
      }
    }

    // Print the number of flows left in the map after each round of termination
    std::cout << "Flows left: " << flows_map.size() << "\n";
  }
}

// Function to add or update a flow
void flow_add_or_update(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port,
                        uint16_t dst_port, int total_bytes, uint8_t protocol,
                        std::string local_addr) {
  std::lock_guard<std::mutex> lock(flows_map_mutex);

  /* !!!DATA COLLECTION ONLY INCLUDES CUSTOM IP!!! */
  if (ip_to_str(dst_ip) != local_addr && ip_to_str(dst_ip) != "192.168.36.1") {
    printf("Skipping flow... (dst IP is not the local IP)\n");
    std::cout << "dst_ip: " << ip_to_str(dst_ip) << std::endl;
    std::cout << "local ip: " << local_addr << std::endl; 
    return;
  }

  // Normalize key based on IP pair and protocol
  FlowKey key = create_normalized_key(src_ip, dst_ip, protocol);
  auto it = flows_map.find(key);

  if (it != flows_map.end()) {
    // Update existing flow
    it->second.total_bytes += total_bytes;
    it->second.packet_count += 1;
    it->second.last_update_time = std::chrono::system_clock::now();

    it->second.duration = std::chrono::duration_cast<std::chrono::seconds>(
        it->second.last_update_time - it->second.start_time);

    // Track source and destination ports
    it->second.src_ports.insert(src_port);
    it->second.dst_ports.insert(dst_port);

    std::cout << "Flow Updated!\n";
  } else {
    // Create new flow with uint32_t for IPs
    Flow flow;
    flow.src_ip = src_ip;
    flow.dst_ip = dst_ip;
    flow.total_bytes = total_bytes;
    flow.protocol = protocol;
    flow.rate = 0.0;
    flow.avg_packet_size = 0.0;
    flow.packet_count = 1;
    flow.start_time = std::chrono::system_clock::now();
    flow.last_update_time = std::chrono::system_clock::now();
    flow.duration = std::chrono::seconds(0);

    flow.src_ports.insert(src_port);
    flow.dst_ports.insert(dst_port);

    flows_map.insert({key, flow});
    std::cout << "Flow Created!\n";
  }

  // Print flow details
  const Flow &value = it != flows_map.end() ? it->second : flows_map[key];
  auto start_time = std::chrono::system_clock::to_time_t(value.start_time);
  auto last_update_time =
      std::chrono::system_clock::to_time_t(value.last_update_time);

  std::cout << "src_ip: " << ip_to_str(value.src_ip) << "\n"
            << "dst_ip: " << ip_to_str(value.dst_ip) << "\n"
            << "protocol: " << static_cast<int>(value.protocol) << "\n"
            << "total_bytes: " << value.total_bytes << "\n"
            << "packet_count: " << value.packet_count << "\n"
            << "src_port: " << src_port << "\n"
            << "dst_port: " << dst_port << "\n"
            << "start_time: "
            << std::put_time(std::localtime(&start_time), "%Y-%m-%d %H:%M:%S")
            << "\n"
            << "last_update_time: "
            << std::put_time(std::localtime(&last_update_time),
                             "%Y-%m-%d %H:%M:%S")
            << "\n"
            << "src_ports: ";
  for (const auto &port : value.src_ports) {
    std::cout << port << " ";
  }
  std::cout << "\ndst_ports: ";
  for (const auto &port : value.dst_ports) {
    std::cout << port << " ";
  }
  std::cout << "\n";
}
