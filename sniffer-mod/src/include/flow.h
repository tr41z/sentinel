#ifndef FLOW_H
#define FLOW_H

#include "ip.h"
#include <time.h>

struct Flow {
  ipv4Ptr src_ip;
  uint16_t src_port;
  ipv4Ptr dst_ip;
  uint16_t dst_port;
  int total_bytes = 0;
  uint8_t protocol;
  time_t start_time;
  time_t last_update_time;
};

struct FlowKey {
  ipv4Ptr src_ip;
  uint16_t src_port;
  ipv4Ptr dst_ip;
  uint16_t dst_port;
  uint8_t protocol;

  bool operator==(const FlowKey &other) const {
    return src_ip == other.src_ip && src_port == other.src_port &&
           dst_ip == other.dst_ip && dst_port == other.dst_port &&
           protocol == other.protocol;
  }
};

#endif
