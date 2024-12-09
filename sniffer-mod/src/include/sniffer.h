#ifndef SNIFFER_H
#define SNIFFER_H

#include "packet.h"
#include <pcap.h>

typedef uint32_t u_int;
typedef uint16_t u_short;
typedef uint8_t u_char;

typedef pcap_if_t *interPtr; /* For better readibility */

interPtr find_devices();          /* Returns interfaces to sniff on */
void start_sniffer(interPtr dev); /* Starts sniffer on device (interface) */

#endif
