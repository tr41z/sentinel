#ifndef SNIFFER_H
#define SNIFFER_H

#include "packet.h"
#include <pcap.h>

typedef pcap_if_t *interPtr; /* For better readibility */

interPtr find_devices();          /* Returns interfaces to sniff on */
void start_sniffer(interPtr dev); /* Starts sniffer on device (interface) */

#endif
