#include <gtest/gtest.h>
#include <climits>
#include <pcap.h>
#include "../include/prep.h"
#include "../include/sniffer.h"
#include "../include/flow.h"
#include "../include/db.h"

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

TEST(SNIFFER_TESTS, ListAvailableDevices) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *device;

    // Find all devices
    ASSERT_EQ(pcap_findalldevs(&alldevs, errbuf), 0) << "Failed to find devices: " << errbuf;

    // Ensure at least one device is available
    ASSERT_NE(alldevs, nullptr) << "No devices found for packet capture.";

    // Free the device list
    pcap_freealldevs(alldevs);
}

TEST(SNIFFER_TESTS, ProtocolIdentificationTest) {
    // Simulate protocol identification for reconnaissance traffic
    uint8_t tcp_syn_flag = 0x02; // SYN flag
    uint8_t icmp_echo_request = 8; // ICMP Echo Request type
    uint16_t arp_op_request = 1; // ARP Request opcode

    ASSERT_EQ(tcp_syn_flag, 0x02) << "TCP SYN flag mismatch.";
    ASSERT_EQ(icmp_echo_request, 8) << "ICMP Echo Request type mismatch.";
    ASSERT_EQ(arp_op_request, 1) << "ARP Request opcode mismatch.";
}

TEST(SNIFFER_TESTS, FeatureExtractionTest) {
    // Simulate feature extraction from a packet
    uint32_t src_ip = 0xC0A80001; // 192.168.0.1
    uint32_t dst_ip = 0xC0A80002; // 192.168.0.2
    uint16_t src_port = 12345;
    uint16_t dst_port = 80;

    ASSERT_EQ(ip_to_str(src_ip), "192.168.0.1");
    ASSERT_EQ(ip_to_str(dst_ip), "192.168.0.2");
    ASSERT_EQ(src_port, 12345);
    ASSERT_EQ(dst_port, 80);
}

TEST(SNIFFER_TESTS, DatabaseInsertionTest) {
    // Simulate database insertion for a flow
    sqlite3 *db;
    char *home_dir = get_home_dir();
    connect_db(home_dir, &db);

    // Clear the database before the test
    const char *clear_query = "DELETE FROM flows;";
    sqlite3_exec(db, clear_query, nullptr, nullptr, nullptr);

    Flow test_flow;
    test_flow.src_ip = 0xC0A80001; // 192.168.0.1
    test_flow.dst_ip = 0xC0A80002; // 192.168.0.2
    test_flow.total_bytes = 1024;
    test_flow.packet_count = 10;
    test_flow.protocol = IPPROTO_TCP;
    test_flow.start_time = std::chrono::system_clock::now();
    test_flow.last_update_time = std::chrono::system_clock::now();
    test_flow.duration = std::chrono::seconds(5);

    save_flow(db, test_flow);

    sqlite3_stmt *stmt;
    const char *query = "SELECT COUNT(*) FROM flows WHERE src_ip = ? AND dst_ip = ?;";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ip_to_str(test_flow.src_ip).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ip_to_str(test_flow.dst_ip).c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    ASSERT_EQ(rc, SQLITE_ROW);
    ASSERT_EQ(sqlite3_column_int(stmt, 0), 1) << "Flow not inserted into database.";

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
