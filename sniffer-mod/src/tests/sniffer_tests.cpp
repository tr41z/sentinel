#include <gtest/gtest.h>
#include <climits>
#include "../include/prep.h"

TEST(PREPROCESS_TESTS_ROUND_TO, Normal) {
   double value = 10.241258125491230542149;
   double res = round_to(value, DECIMAL_PLACES);

   ASSERT_EQ(10.24125813, res);
}

TEST(PREPROCESS_TESTS_COUNT_PORTS, WellKnownPortRange) {
    std::unordered_set<uint16_t> set {22, 53, 80, 8080, 443, 5432, 3258, 2489, 9944};
    int low = 0;
    int high = 1023;
    
    int res = count_ports_in_range(set, low, high);
    ASSERT_EQ(4, res);
}

TEST(PREPROCESS_TESTS_COUNT_PORTS, RegisteredPortRange) {
    std::unordered_set<uint16_t> set {22, 53, 80, 8080, 443, 5432, 3258, 2489, 9944};
    int low = 1024;
    int high = 49151;
    
    int res = count_ports_in_range(set, low, high);
    ASSERT_EQ(5, res);
}

TEST(PREPROCESS_TESTS_COUNT_PORTS, DynamicPortRange) {
    std::unordered_set<uint16_t> set {22, 53, 80, 8080, 443, 5432, 3258, 2489, 9944};
    int low = 49152;
    int high = 65535;
    
    int res = count_ports_in_range(set, low, high);
    ASSERT_EQ(0, res);
}

TEST(PREPROCESS_TESTS_COUNT_PORTS, MaxRange) {
    std::unordered_set<uint16_t> set;
    int low = 0;
    int high = 65535;

    // All possible ports in set
    for (int i = 0; i <= 65535; i++) {
        set.insert(i);
    }

    int res = count_ports_in_range(set, low, high);
    ASSERT_EQ(65536, res);
}

TEST(PREPROCESS_TESTS_COUNT_PORTS, MinRange) {
    std::unordered_set<uint16_t> set {};
    int low = 0;
    int high = 1023;

    int res = count_ports_in_range(set, low, high);
    ASSERT_EQ(0, res);
}

TEST(PREPROCESS_TESTS_PACKETS_PER_SEC, Normal) {
    int packet_count = 84;
    int duration = 5;

    double res = calculate_pps(packet_count, duration);
    ASSERT_EQ(16.8, res);
}

TEST(PREPROCESS_TESTS_PACKETS_PER_SEC, ZeroDivision) {
    int packet_count = 955;
    int duration = 0;

    double res = calculate_pps(packet_count, duration);
    ASSERT_EQ(0, res);
}

TEST(PREPROCESS_TESTS_PACKETS_PER_SEC, MaxValues) {
    int packet_count = 991200000;
    int duration = 45;

    // Anomaly in packet count
    double pRes = calculate_pps(INT_MAX, duration);
    
    // Anomaly in duration
    double dRes = calculate_pps(packet_count, INT_MAX);
    
    ASSERT_NEAR(47721858.82222222, pRes, 0.01);
    ASSERT_NEAR(0.46156346819, dRes, 0.001);
}

TEST(PREPROCESS_TESTS_PACKETS_PER_SEC, MinValues) {
    int packet_count = 0;
    int duration = 1;

    double res = calculate_pps(packet_count, duration);
    ASSERT_EQ(0, res);
}

TEST(PREPROCESS_TESTS_IS_BRUTE_TARGET, AllPossible) {
    std::unordered_set<uint16_t> ports 
        {23, 22, 1, 503, 454, 123, 21, 3389, 445, 5900, 9988};

    int res = is_brute_target(ports);
    ASSERT_EQ(1, res);
}

TEST(PREPROCESS_TESTS_IS_BRUTE_TARGET, None) {
    std::unordered_set<uint16_t> ports
        {8080, 4444, 3333, 2222, 2821, 5823, 1292};

    int res = is_brute_target(ports);
    ASSERT_EQ(0, res);
}

TEST(PREPROCESS_TESTS_IS_BRUTE_TARGET, EmptyPorts) {
    std::unordered_set<uint16_t> ports {};

    int res = is_brute_target(ports);
    ASSERT_EQ(0, res);
}

TEST(PREPROCESS_TESTS_IS_DOS_TARGET, AllPossible) {
    std::unordered_set<uint16_t> ports 
        {80, 443, 53, 25, 143, 110, 22, 23, 3389, 21, 161, 162, 5060, 5061, 27015, 3074};

    int res = is_dos_target(ports);
    ASSERT_EQ(1, res);
}

TEST(PREPROCESS_TESTS_IS_DOS_TARGET, None) {
    std::unordered_set<uint16_t> ports
        {8080, 4444, 3333, 2222, 2821, 5823, 1292};

    int res = is_dos_target(ports);
    ASSERT_EQ(0, res);
}

TEST(PREPROCESS_TESTS_IS_DOS_TARGET, EmptyPorts) {
    std::unordered_set<uint16_t> ports {};

    int res = is_dos_target(ports);
    ASSERT_EQ(0, res);
}
