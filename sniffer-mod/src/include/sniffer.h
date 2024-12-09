#include <stdint.h>       // Add this to define uint32_t, uint16_t, uint8_t
typedef uint16_t u_short; // Define u_short as uint16_t if needed
typedef uint8_t u_char;   // Define u_char as uint8_t if needed

#ifndef SNIFFER_H
#define SNIFFER_H

#include "packet.h"
#include <pcap.h>

typedef pcap_if_t *interPtr; /* For better readibility */

interPtr find_devices();          /* Returns interfaces to sniff on */
void start_sniffer(interPtr dev); /* Starts sniffer on device (interface) */

#endif
