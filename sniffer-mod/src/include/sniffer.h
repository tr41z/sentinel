#ifndef SNIFFER_H
#define SNIFFER_H

#include "packet.h"
#include <pcap/pcap.h>

#ifndef u_int
typedef unsigned int u_int;
#endif

#ifndef u_short
typedef unsigned short u_short;
#endif

#ifndef u_char
typedef unsigned char u_char;
#endif

typedef pcap_if_t *interPtr; /* For better readibility */

interPtr find_devices();          /* Returns interfaces to sniff on */
void start_sniffer(interPtr dev); /* Starts sniffer on device (interface) */

#endif
