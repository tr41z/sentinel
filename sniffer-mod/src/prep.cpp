#include "include/prep.h"

int count_ports_in_range(portSet ports, int low, int high) {
    int count = 0;

    while (high > low) {
        for (int i = 0; i <= high; i++) {
            if (ports.find(i) != ports.end()) {
                count++;
                low++;
            }
            low++;
        }
    }
    return count;
}
