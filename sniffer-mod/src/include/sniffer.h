#include <stdint.h>

typedef uint32_t u_int;   /* Define u_int as uint32_t */
typedef uint16_t u_short; /* Define u_short as uint16_t */
typedef uint8_t u_char;   /* Define u_char as uint8_t */

#ifndef SNIFFER_H
#define SNIFFER_H

#include "packet.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <pcap.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PACKET_ARRIVAL_THRESHOLD 20

typedef struct {
  char *device_name;
} ThreadArgs;

void *sniffer_thread(void *args);
void start_multithreaded_sniffing();

#ifdef __cplusplus
}
#endif

#endif
