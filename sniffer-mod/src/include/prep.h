#ifndef PREP_H
#define PREP_H

#include <unordered_set>
#include <cstdint>

#define DECIMAL_PLACES 8

double round_to(double value, int decimal_places);
int count_ports_in_range(std::unordered_set<uint16_t> ports, int low, int high);
double calculate_pps(int total_packet_count, int duration); // Packets per sec
double calculate_bpp(int total_bytes, int total_packet_count); // Bytes per packet
double calculate_rate(int total_bytes, int duration);
int is_brute_target(std::unordered_set<uint16_t> ports);
int is_dos_target(std::unordered_set<uint16_t> ports);

#endif /* PREP_H */
