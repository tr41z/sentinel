#!/bin/bash

# Function to run Nmap with various flags for reconnaissance
run_nmap() {
    local target_ip=$1
    nmap_flags=(
        "-sS"  # SYN scan
        "-sU"  # UDP scan
        "-p-"  # Scan all ports
        "-A"   # Aggressive scan (OS detection, version detection, script scanning)
        "-O"   # OS detection
        "-sV"  # Version detection
        "-T4"  # Faster scan
    )

    # Randomly select between 2 to 4 flags
    selected_flags=()
    num_flags=$((RANDOM % 3 + 2)) # Random number between 2 and 4

    for ((i = 0; i < num_flags; i++)); do
        flag=${nmap_flags[RANDOM % ${#nmap_flags[@]}]}
        # Ensure the flag is not duplicated
        [[ " ${selected_flags[@]} " =~ " $flag " ]] || selected_flags+=("$flag")
    done

    echo "Running Nmap with flags: ${selected_flags[*]}"
    nmap "${selected_flags[@]}" $target_ip

    # Random delay between 5 and 10 seconds
    sleep $((RANDOM % 6 + 5))
}

# Function to run Gobuster (subdomain brute force)
run_gobuster() {
    local target_domain=$1
    echo "Running Gobuster for domain $target_domain..."
    gobuster dir -u $target_domain -w /usr/share/wordlists/dirb/common.txt
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
    ./dirgo -u $target_domain:80 -w /usr/share/wordlists/dirb/common.txt -t 20
    sleep $((RANDOM % 6 + 5))
}

run_wfuzz() {
    local target_domain=$1
    echo "Running WFuzz for domain $target_domain..."
    wfuzz -c -z file,/usr/share/wordlists/dirb/common.txt --hc 404 $target_domain/FUZZ
    sleep $((RANDOM % 6 + 5))
}

run_metasploit_scan() {
    local target_ip=$1
    echo "Running Metasploit auxiliary scanner for $target_ip..."
    msfconsole -q -x "use auxiliary/scanner/portscan/tcp; set RHOSTS $target_ip; run; exit"
    sleep $((RANDOM % 6 + 5))
}

run_nikto() {
    local target_domain=$1
    echo "Running Nikto for domain $target_domain..."
    echo "n" | nikto -h $target_domain -nointeractive
    sleep $((RANDOM % 6 + 5))
}

# Main function to run reconnaissance tools at random intervals
main() {
    local target_ip="192.168.1.102"
    local target_domain="http://192.168.1.102"

    # Run reconnaissance tools in an infinite loop with random intervals
    echo "Running reconnaissance attacks on $target_domain and $target_ip..."

    tools=("run_nmap" "run_gobuster" "run_masscan" "run_dirgo" "run_wfuzz" "run_metasploit_scan" "run_nikto")

    while true; do
        # Randomly select a tool to run
        tool=${tools[$RANDOM % ${#tools[@]}]}

        # Run the selected tool
        $tool $target_ip $target_domain

        # Random delay between 2 to 6 minutes (120 to 360 seconds)
        random_delay=$((RANDOM % 31 + 30))
        echo "Waiting for $random_delay seconds before running another attack..."
        sleep $random_delay
    done
}

# Run the main function
main
