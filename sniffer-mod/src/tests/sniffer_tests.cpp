#include <gtest/gtest.h>
#include <limits>
#include "../include/prep.h"

TEST(PREPROCESS_TESTS_COUNT_PORTS, WellKnownPortRange) {
    portSet set {22, 53, 80, 8080, 443, 5432, 3258, 2489, 9944};
    int low = 0;
    int high = 1023;
    
    int res = count_ports_in_range(set, low, high);
    ASSERT_EQ(4, res);
}

TEST(PREPROCESS_TESTS_COUNT_PORTS, RegisteredPortRange) {
    portSet set {22, 53, 80, 8080, 443, 5432, 3258, 2489, 9944};
    int low = 1024;
    int high = 49151;
    
    int res = count_ports_in_range(set, low, high);
    ASSERT_EQ(5, res);
}

TEST(PREPROCESS_TESTS_COUNT_PORTS, DynamicPortRange) {
    portSet set {22, 53, 80, 8080, 443, 5432, 3258, 2489, 9944};
    int low = 49152;
    int high = 65535;
    
    int res = count_ports_in_range(set, low, high);
    ASSERT_EQ(0, res);
}

TEST(PREPROCESS_TESTS_COUNT_PORTS, MaxRange) {
    portSet set;
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
    portSet set {};
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
    ASSERT_EQ(955, res);
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

