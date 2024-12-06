#ifndef FLOW_H
#define FLOW_H

struct Flow {
  char *src_ip;
  char *dst_ip;
  int total_bytes;
  int protocol;
};

#endif
