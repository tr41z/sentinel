#include "include/prep.h"
#include <cmath>
#include <vector>

double round_to(double value, int decimal_places) {
    double factor = std::pow(10.0, decimal_places);
    return std::round(value * factor) / factor;
} 

int count_ports_in_range(std::unordered_set<uint16_t> ports, int low, int high) {
    int count = 0;
    
    for (int i = low; i <= high; i++) {
        if (ports.find(i) != ports.end()) {
            count++;
        }
    }
    return count;
}

double calculate_pps(int total_packet_count, int duration) {
    return duration > 0 ?
        round_to(static_cast<double>(total_packet_count) / duration, DECIMAL_PLACES) 
        : 0.0; 
}

double calculate_bpp(int total_bytes, int total_packet_count) {
    return total_packet_count > 0 ? 
        round_to(static_cast<double>(total_bytes) / total_packet_count, DECIMAL_PLACES) 
        : 0.0;
}

double calculate_rate(int total_bytes, int duration) {
    return duration > 0 ?
        round_to(static_cast<double>(total_bytes) / duration, DECIMAL_PLACES) 
        : 0.0;
}

int is_brute_target(std::unordered_set<uint16_t> ports) {
    std::vector<int> commonBrutePorts 
        {22, 23, 21, 3389, 445, 5900};

    for (int i = 0; i <= commonBrutePorts.size(); i++) {
       if (ports.find(commonBrutePorts[i]) != ports.end()) {
           return 1;
       }
    }

    return 0;
}

int is_dos_target(std::unordered_set<uint16_t> ports) {
    std::vector<int> commonDosPorts 
        {80, 443, 53, 25, 143, 110, 22, 23, 3389, 21, 161, 162, 5060, 5061, 27015, 3074};
   
    for (int i = 0; i <= commonDosPorts.size(); i++) {
        if (ports.find(commonDosPorts[i]) != ports.end()) {
            return 1;
        }
    }

    return 0;
}
