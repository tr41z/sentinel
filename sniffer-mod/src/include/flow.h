#ifndef FLOW_H
#define FLOW_H

#include "packet.h"
#include <stdint.h>
#include <time.h>

struct Flow {
  char *src_ip;
  uint16_t src_port;
  char *dst_ip;
  uint16_t dst_port;
  int total_bytes;
  PacketProtocol protocol;
  time_t *startTime;
  time_t *lastUpdateTime;
};

struct FlowKey {
  char *src_ip;
  uint16_t src_port;
  char *dst_ip;
  uint16_t dst_port;
  PacketProtocol protocol;
};

struct FlowsMap {
  FlowKey flow;
  bool terminated;
  /* Custom dtype for adding / terminating flows by `FlowKey`
   * and length of idle state.
   * IF lastUpdateTime - startTime <= 300 seconds {
   *   terminated = true;
   *   create current flow as Flow struct
   * }
   * */
};

#endif
