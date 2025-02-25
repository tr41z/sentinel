#ifndef PREP_H
#define PREP_H

#include <unordered_set>

typedef std::unordered_set<uint16_t> portSet;

int count_ports_in_range(portSet ports, int low, int high);
int count_unique_ports(portSet ports);
float calculate_entropy(portSet ports);
float calculate_pps(int total_packet_count, int duration); // Packets per sec
float calculate_bps(int total_bytes, int duration); // Bytes per sec
float calculate_avg_bpp(int total_bytes, int total_packet_count); // Bytes per packet
int is_brute_target(portSet ports);
int is_dos_target(portSet ports);

#endif /* PREP_H */
