#include "include/prep.h"
#include <cmath>

double round_to(double value, int decimal_places) {
    double factor = std::pow(10.0, decimal_places);
    return std::round(value * factor) / factor;
} 

int count_ports_in_range(portSet ports, int low, int high) {
    if (ports.empty()) {
        return 0;
    }

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
        round_to(static_cast<double>(total_packet_count) / duration, 5) 
        : total_packet_count; 
}

