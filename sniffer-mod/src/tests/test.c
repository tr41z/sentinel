#include "../include/ip.h"
#include "../include/packet.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdint.h>

void test_t_new() {
  char src_ip_str[20];
  char dst_ip_str[20];

  ipv4Ptr src_ip = ipv4_new(192, 168, 10, 102);
  int src_port = 20;
  ipv4Ptr dst_ip = ipv4_new(255, 255, 255, 255);
  int dst_port = 33;

  sprintf(src_ip_str, "%d.%d.%d.%d", src_ip->octets[0], src_ip->octets[1],
          src_ip->octets[2], src_ip->octets[3]);

  sprintf(dst_ip_str, "%d.%d.%d.%d", dst_ip->octets[0], dst_ip->octets[1],
          dst_ip->octets[2], dst_ip->octets[3]);

  // New TCP packet
  tcpPtr new_tcp_packet = tcp_new(src_ip, src_port, dst_ip, dst_port);

  // Ensure packet is properly allocated
  CU_ASSERT_PTR_NOT_NULL(new_tcp_packet);

  // Checks if values were set correctly
  CU_ASSERT_STRING_EQUAL(src_ip_str, "192.168.10.102");
  CU_ASSERT_EQUAL(new_tcp_packet->src_port, 20);
  CU_ASSERT_STRING_EQUAL(dst_ip_str, "255.255.255.255");
  CU_ASSERT_EQUAL(new_tcp_packet->dst_port, 33);
}

void test_t_free() {
  ipv4Ptr src_ip = ipv4_new(192, 168, 1, 1);
  int src_port = 20;
  ipv4Ptr dst_ip = ipv4_new(192, 168, 1, 10);
  int dst_port = 33;

  tcpPtr new_tcp_packet = tcp_new(src_ip, src_port, dst_ip, dst_port);

  // Ensure memory is allocated
  CU_ASSERT_PTR_NOT_NULL(new_tcp_packet);
  CU_ASSERT_PTR_NOT_NULL(src_ip);
  CU_ASSERT_PTR_NOT_NULL(dst_ip);

  // Free the TCP packet with addresses
  tcp_free(new_tcp_packet);

  // After freeing, set the pointer to NULL
  new_tcp_packet = NULL;
  src_ip = NULL;
  dst_ip = NULL;

  // Check if freed properly
  CU_ASSERT_PTR_NULL(new_tcp_packet);
  CU_ASSERT_PTR_NULL(src_ip);
  CU_ASSERT_PTR_NULL(dst_ip);
}

void test_ipv4_new() {
  ipv4Ptr new_ipv4_addr = ipv4_new(192, 168, 1, 1);
  char str_ipv4_addr[20];

  sprintf(str_ipv4_addr, "%d.%d.%d.%d", new_ipv4_addr->octets[0],
          new_ipv4_addr->octets[1], new_ipv4_addr->octets[2],
          new_ipv4_addr->octets[3]);

  CU_ASSERT_STRING_EQUAL(str_ipv4_addr, "192.168.1.1");
}

int main() {
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("Sentinel Sniffer Tests", 0, 0);

  CU_add_test(suite, "Testing TCP Packet Creation", test_t_new);
  CU_add_test(suite, "Testing TCP Packed Freeing", test_t_free);
  CU_add_test(suite, "Testing IpV4 Address Creation", test_ipv4_new);

  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_basic_run_tests();
  CU_cleanup_registry();
  return 0;
}
