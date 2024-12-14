#include "../include/db.h"
#include "../include/ip.h"
#include "../include/packet.h"
#include "../include/sniffer.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

void test_connect_string() {
  char *result = connect_string("Hello ", "World!");

  CU_ASSERT_STRING_EQUAL(result, "Hello World!");

  free(result);
  result = NULL;

  CU_ASSERT_PTR_NULL(result);
}

void test_connect_string_path() {
  char *home_dir = "/Users/profile";
  char *db_path = "/.sentinel/sentinel.db";

  char *result = connect_string(home_dir, db_path);

  CU_ASSERT_STRING_EQUAL(result, "/Users/profile/.sentinel/sentinel.db");
}

void test_connect_string_edge_case() {
  char *result = connect_string("", "");

  CU_ASSERT_STRING_EQUAL(result, "");

  free(result);
  result = NULL;

  CU_ASSERT_PTR_NULL(result);
}

void test_ipv4_new() {
  char str_ipv4_addr[20];
  ipv4Ptr new_ipv4_addr = ipv4_new(192, 168, 1, 1);

  sprintf(str_ipv4_addr, "%d.%d.%d.%d", new_ipv4_addr->octets[0],
          new_ipv4_addr->octets[1], new_ipv4_addr->octets[2],
          new_ipv4_addr->octets[3]);

  CU_ASSERT_STRING_EQUAL(str_ipv4_addr, "192.168.1.1");

  ipv4_free(new_ipv4_addr);
  new_ipv4_addr = NULL;

  CU_ASSERT_PTR_NULL(new_ipv4_addr);
}

void test_ipv4_new_edge_case() {
  ipv4Ptr ipv4_addr;

  ipv4_addr = ipv4_new(0, 0, 0, 0);
  CU_ASSERT_PTR_NOT_NULL(ipv4_addr);

  ipv4_free(ipv4_addr);
  ipv4_addr = NULL;

  ipv4_addr = ipv4_new(255, 255, 255, 255);
  CU_ASSERT_PTR_NOT_NULL(ipv4_addr);

  ipv4_free(ipv4_addr);
  ipv4_addr = NULL;

  CU_ASSERT_PTR_NULL(ipv4_addr);
}

void test_ipv4_free() {
  ipv4Ptr src_ip = ipv4_new(192, 168, 10, 102);
  ipv4Ptr dst_ip = ipv4_new(192, 255, 255, 255);

  CU_ASSERT_PTR_NOT_NULL(src_ip);
  CU_ASSERT_PTR_NOT_NULL(dst_ip);

  ipv4_free(src_ip);
  ipv4_free(dst_ip);

  src_ip = NULL;
  dst_ip = NULL;

  CU_ASSERT_PTR_NULL(src_ip);
  CU_ASSERT_PTR_NULL(dst_ip);
}

void test_cmb_new() {
  char src_ip_str[20];
  char dst_ip_str[20];

  ipv4Ptr src_ip = ipv4_new(192, 168, 10, 102);
  uint16_t src_port = 20;
  ipv4Ptr dst_ip = ipv4_new(192, 168, 1, 1);
  uint16_t dst_port = 33;

  sprintf(src_ip_str, "%d.%d.%d.%d", src_ip->octets[0], src_ip->octets[1],
          src_ip->octets[2], src_ip->octets[3]);

  sprintf(dst_ip_str, "%d.%d.%d.%d", dst_ip->octets[0], dst_ip->octets[1],
          dst_ip->octets[2], dst_ip->octets[3]);

  // New Combined Packet
  ipPtr ip_header =
      ip_new(4, 30, 40, 2324, 1, 3, 88, 64, 17, 333, src_ip, dst_ip);
  cmbPtr cmb_packet = cmb_new(ip_header, src_port, dst_port, 30);

  // Ensure packet is properly allocated
  CU_ASSERT_PTR_NOT_NULL(cmb_packet);

  // Checks if values were set correctly
  CU_ASSERT_STRING_EQUAL(src_ip_str, "192.168.10.102");
  CU_ASSERT_EQUAL(cmb_packet->src_port, 20);
  CU_ASSERT_STRING_EQUAL(dst_ip_str, "192.168.1.1");
  CU_ASSERT_EQUAL(cmb_packet->dst_port, 33);

  cmb_free(cmb_packet);
  cmb_packet = NULL;

  CU_ASSERT_PTR_NULL(cmb_packet);
}

void test_cmb_new_edge_case() {
  char src_ip_str[20];
  char dst_ip_str[20];

  ipv4Ptr src_ip = ipv4_new(0, 0, 0, 0);
  ipv4Ptr dst_ip = ipv4_new(255, 255, 255, 255);
  uint16_t src_port = 0;
  uint16_t dst_port = 65535;

  sprintf(src_ip_str, "%d.%d.%d.%d", src_ip->octets[0], src_ip->octets[1],
          src_ip->octets[2], src_ip->octets[3]);

  sprintf(dst_ip_str, "%d.%d.%d.%d", dst_ip->octets[0], dst_ip->octets[1],
          dst_ip->octets[2], dst_ip->octets[3]);

  // New Combined Packet
  ipPtr ip_header =
      ip_new(4, 30, 40, 2324, 1, 3, 88, 64, 6, 333, src_ip, dst_ip);
  cmbPtr cmb_packet = cmb_new(ip_header, src_port, dst_port, 20);

  // Ensure packet is properly allocated
  CU_ASSERT_PTR_NOT_NULL(cmb_packet);

  // Check if values were set correctly
  CU_ASSERT_STRING_EQUAL(src_ip_str, "0.0.0.0");
  CU_ASSERT_EQUAL(cmb_packet->src_port, 0);
  CU_ASSERT_STRING_EQUAL(dst_ip_str, "255.255.255.255");
  CU_ASSERT_EQUAL(cmb_packet->dst_port, 65535);

  // IPs are freed by freeing cmb packet as it has full ownership
  cmb_free(cmb_packet);
  cmb_packet = NULL;

  CU_ASSERT_PTR_NULL(cmb_packet);
}

void test_cmb_free() {
  ipv4Ptr src_ip = ipv4_new(192, 168, 1, 1);
  uint16_t src_port = 20;
  ipv4Ptr dst_ip = ipv4_new(192, 168, 1, 10);
  uint16_t dst_port = 33;

  // New Combined Packet
  ipPtr ip_header =
      ip_new(4, 30, 40, 2324, 1, 3, 88, 64, 17, 333, src_ip, dst_ip);
  cmbPtr cmb_packet = cmb_new(ip_header, src_port, dst_port, 20);

  // Ensure memory is allocated
  CU_ASSERT_PTR_NOT_NULL(cmb_packet);
  CU_ASSERT_PTR_NOT_NULL(src_ip);
  CU_ASSERT_PTR_NOT_NULL(dst_ip);

  // Free the Combined packet with addresses
  cmb_free(cmb_packet);

  // After freeing, set the pointer to NULL
  cmb_packet = NULL;

  // Check if freed properly
  CU_ASSERT_PTR_NULL(cmb_packet);
}

void test_ip_new() {
  ipv4Ptr src_ip = ipv4_new(192, 168, 1, 1);
  ipv4Ptr dst_ip = ipv4_new(192, 168, 1, 10);

  CU_ASSERT_PTR_NOT_NULL(src_ip);
  CU_ASSERT_PTR_NOT_NULL(dst_ip);

  // New Combined Packet
  ipPtr ip_header =
      ip_new(4, 30, 40, 2324, 1, 3, 88, 64, 17, 333, src_ip, dst_ip);

  CU_ASSERT_PTR_NOT_NULL(ip_header);

  ip_free(ip_header);
  src_ip = NULL;
  dst_ip = NULL;
  ip_header = NULL;
}

void test_ip_new_edge_case() {
  ipv4Ptr src_ip = ipv4_new(0, 0, 0, 0);
  ipv4Ptr dst_ip = ipv4_new(255, 255, 255, 255);

  CU_ASSERT_PTR_NOT_NULL(src_ip);
  CU_ASSERT_PTR_NOT_NULL(dst_ip);

  // New Combined Packet
  ipPtr ip_header = ip_new(255, 255, 255, 65535, 65535, 255, 65535, 255, 255,
                           65535, src_ip, dst_ip);

  CU_ASSERT_PTR_NOT_NULL(ip_header);

  ip_free(ip_header);
  src_ip = NULL;
  dst_ip = NULL;
  ip_header = NULL;
}

void test_ip_free() {
  ipv4Ptr src_ip = ipv4_new(0, 0, 0, 0);
  ipv4Ptr dst_ip = ipv4_new(255, 255, 255, 255);

  CU_ASSERT_PTR_NOT_NULL(src_ip);
  CU_ASSERT_PTR_NOT_NULL(dst_ip);

  // New Combined Packet
  ipPtr ip_header = ip_new(255, 255, 255, 65535, 65535, 255, 65535, 255, 255,
                           65535, src_ip, dst_ip);

  CU_ASSERT_PTR_NOT_NULL(ip_header);

  ip_free(ip_header);
  src_ip = NULL;
  dst_ip = NULL;
  ip_header = NULL;

  CU_ASSERT_PTR_NULL(ip_header);
  CU_ASSERT_PTR_NULL(src_ip);
  CU_ASSERT_PTR_NULL(dst_ip);
}

int main() {
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("Sentinel Sniffer Tests", 0, 0);

  CU_add_test(suite, "Testing IPV4 Address Creation", test_ipv4_new);
  CU_add_test(suite, "Testing IPV4 Address Freeing", test_ipv4_free);
  CU_add_test(suite, "Testing IPV4 Address Creation Edge Cases",
              test_ipv4_new_edge_case);
  CU_add_test(suite, "Testing CMB Packet Creation", test_cmb_new);
  CU_add_test(suite, "Testing CMB Packet Freeing", test_cmb_free);
  CU_add_test(suite, "Testing CMB Packet Edge Cases", test_cmb_new_edge_case);
  CU_add_test(suite, "Testing IP Header Creation", test_ip_new);
  CU_add_test(suite, "Testing IP Header Creation Edge Cases",
              test_ip_new_edge_case);
  CU_add_test(suite, "Testing IP Header Freeing", test_ip_free);
  CU_add_test(suite, "Testing String Connection", test_connect_string);
  CU_add_test(suite, "Testing String Connection Edge Cases",
              test_connect_string_edge_case);
  CU_add_test(suite, "Testing String Connection with Path",
              test_connect_string_path);
  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_basic_run_tests();
  CU_cleanup_registry();
  return 0;
}
