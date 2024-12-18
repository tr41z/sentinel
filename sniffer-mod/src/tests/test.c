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

  free(result);
  result = NULL;

  CU_ASSERT_PTR_NULL(result);
}

void test_connect_string_edge_case() {
  char *result = connect_string("", "");

  CU_ASSERT_STRING_EQUAL(result, "");

  free(result);
  result = NULL;

  CU_ASSERT_PTR_NULL(result);
}

int main() {
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("Sentinel Sniffer Tests", 0, 0);

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
