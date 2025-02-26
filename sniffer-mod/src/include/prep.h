#ifndef PREP_H
#define PREP_H

#include <unordered_set>
#include <cstdint>

typedef std::unordered_set<uint16_t> portSet;

double round_to(double value, int decimal_places);
int count_ports_in_range(portSet ports, int low, int high);
double calculate_pps(int total_packet_count, int duration); // Packets per sec
double calculate_avg_bpp(int total_bytes, int total_packet_count); // Bytes per packet
double calculate_rate(int total_bytes, int duration);
int is_brute_target(portSet ports);
int is_dos_target(portSet ports);

#endif /* PREP_H */
