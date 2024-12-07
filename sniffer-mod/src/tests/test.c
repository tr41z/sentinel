#include "../include/packet.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

void test_t_new() {
  char *src_ip = "192.168.1.1";
  int src_port = 20;
  char *dst_ip = "192.168.1.102";
  int dst_port = 33;

  // New TCP packet
  tcpPtr new_tcp_packet = t_new(src_ip, src_port, dst_ip, dst_port);

  // Ensure packet is properly allocated
  CU_ASSERT_PTR_NOT_NULL(new_tcp_packet);

  // Checks if values were set correctly
  CU_ASSERT_STRING_EQUAL(new_tcp_packet->src_ip, "192.168.1.1");
  CU_ASSERT_EQUAL(new_tcp_packet->src_port, 20);
  CU_ASSERT_STRING_EQUAL(new_tcp_packet->dst_ip, "192.168.1.102");
  CU_ASSERT_EQUAL(new_tcp_packet->dst_port, 33);
}

void test_t_free() {
  char *src_ip = "192.168.1.1";
  int src_port = 20;
  char *dst_ip = "192.168.1.102";
  int dst_port = 33;

  // New TCP packet
  tcpPtr new_tcp_packet = t_new(src_ip, src_port, dst_ip, dst_port);

  // Ensure memory is allocated
  CU_ASSERT_PTR_NOT_NULL(new_tcp_packet);

  // Free the TCP packet
  t_free(new_tcp_packet);

  // After freeing, set the pointer to NULL
  new_tcp_packet = NULL;

  // Check if freed properly
  CU_ASSERT_PTR_NULL(new_tcp_packet);
}

int main() {
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("Sentinel Sniffer Tests", 0, 0);

  CU_add_test(suite, "Testing TCP Packet Creation", test_t_new);
  CU_add_test(suite, "Testing TCP Packed Freeing", test_t_free);
  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_basic_run_tests();
  CU_cleanup_registry();
  return 0;
}
