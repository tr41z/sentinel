#include <stdint.h> /* For standard types like uint32_t, uint16_t, uint8_t */

typedef uint32_t u_int;   /* Define u_int as uint32_t */
typedef uint16_t u_short; /* Define u_short as uint16_t */
typedef uint8_t u_char;   /* Define u_char as uint8_t */

#ifndef SNIFFER_H
#define SNIFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEV_NUM_PACKETS 1
#define PACKET_ARRIVAL_THRESHOLD 5

#include "packet.h"
#include <pcap.h>

typedef struct {
  char *device_name;
} ThreadArgs;
void *sniffer_thread(void *args);
void start_multithreaded_sniffing();
typedef pcap_if_t *devPtr; /* For better readibility */

devPtr find_devices();          /* Returns interfaces to sniff on */
void start_sniffer(devPtr dev); /* Starts sniffer on device (interface) */

#ifdef __cplusplus
}
#endif

#endif
