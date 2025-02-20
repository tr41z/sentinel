#!/bin/bash

# Function to run Nmap with various flags for reconnaissance
run_nmap() {
    local target_ip=$1
    nmap_flags=(
        "-sS"  # SYN scan
        "-sU"  # UDP scan
        "-p-"   # Scan all ports
        "-A"   # Aggressive scan (OS detection, version detection, script scanning)
        "-O"   # OS detection
        "-sV"  # Version detection
        "-T4"  # Faster scan
    )

    for flag in "${nmap_flags[@]}"; do
        echo "Running Nmap with flag: $flag"
        nmap $flag $target_ip
        sleep $((RANDOM % 6 + 5))  
    done
}

# Function to run Gobuster (subdomain brute force)
run_gobuster() {
    local target_domain=$1
    echo "Running Gobuster for domain $target_domain..."
    gobuster dir -u $target_domain -w /usr/share/wordlists/dirb/common.txt
    sleep $((RANDOM % 6 + 5))  
}

# Function to run Sublist3r (subdomain enumeration)
run_sublist3r() {
    local target_domain=$1
    echo "Running Sublist3r for $target_domain..."
    sublist3r -d $target_domain
    sleep $((RANDOM % 6 + 5))  
}

# Function to run Amass (subdomain enumeration and OSINT)
run_amass() {
    local target_domain=$1
    echo "Running Amass for $target_domain..."
    amass enum -d $target_domain
    sleep $((RANDOM % 6 + 5)) 
}

# Function to run Masscan (fast port scanning)
run_masscan() {
    local target_ip=$1
    echo "Running Masscan for $target_ip..."
    masscan $target_ip -p0-65535 --rate=1000
    sleep $((RANDOM % 6 + 5))  
}

run_dirgo() {
    local target_ip=$1
    echo "Running dirgo for $target_ip..."
    ./dirgo -u $target_domain -w /usr/share/wordlists/dirb/common.txt -t 20
    sleep $((RANDOM % 6 + 5))
}

# Main function to run reconnaissance tools at random intervals
main() {
    local target_ip="192.168.1.102"
    local target_domain="http://192.168.1.102:80"

    # Run reconnaissance tools in an infinite loop with random intervals
    echo "Running reconnaissance attacks on $target_domain and $target_ip..."

    tools=("run_nmap" "run_gobuster" "run_sublist3r" "run_amass" "run_masscan")

    while true; do
        # Randomly select a tool to run
        tool=${tools[$RANDOM % ${#tools[@]}]}

        # Run the selected tool
        $tool $target_ip $target_domain

        # Random delay between 2 to 6 minutes (120 to 360 seconds)
        random_delay=$((RANDOM % 241 + 120))
        echo "Waiting for $random_delay seconds before running another attack..."
        sleep $random_delay
    done
}

# Run the main function
main
