#!/bin/bash

# Log file with timestamp
LOG_FILE="recon_log_$(date '+%Y%m%d_%H%M%S').txt"

# Function to log attacks
log_attack() {
    echo "$(date) - Executed $1 on $2" | tee -a "$LOG_FILE"
}

# Function to run Nmap with various flags for reconnaissance
run_nmap() {
    local target_ip=$1
    nmap_flags=(
        "-sS"  "-sU"  "-p-"  "-A"  "-O"  "-sV"  "-T4"
    )

    # Randomly select between 2 to 4 flags
    selected_flags=()
    num_flags=$((RANDOM % 3 + 2))

    for ((i = 0; i < num_flags; i++)); do
        flag=${nmap_flags[RANDOM % ${#nmap_flags[@]}]}
        [[ " ${selected_flags[@]} " =~ " $flag " ]] || selected_flags+=("$flag")
    done

    echo "Running Nmap with flags: ${selected_flags[*]} on $target_ip" | tee -a "$LOG_FILE"
    nmap "${selected_flags[@]}" "$target_ip" 2>&1 | tee -a "$LOG_FILE"
    log_attack "Nmap (${selected_flags[*]})" "$target_ip"
    sleep $((RANDOM % 6 + 5))
}

# Function to run Gobuster (subdomain brute force)
run_gobuster() {
    local target_domain=$1
    echo "Running Gobuster for domain $target_domain..." | tee -a "$LOG_FILE"
    gobuster dir -u "$target_domain" -w /usr/share/wordlists/dirb/common.txt 2>&1 | tee -a "$LOG_FILE"
    log_attack "Gobuster" "$target_domain"
    sleep $((RANDOM % 6 + 5))
}

# Function to run Masscan (fast port scanning)
run_masscan() {
    local target_ip=$1
    echo "Running Masscan for $target_ip..." | tee -a "$LOG_FILE"
    masscan "$target_ip" -p0-65535 --rate=1000 2>&1 | tee -a "$LOG_FILE"
    log_attack "Masscan" "$target_ip"
    sleep $((RANDOM % 6 + 5))
}

run_dirgo() {
    local target_ip=$1
    echo "Running Dirgo for $target_ip..." | tee -a "$LOG_FILE"
    ./dirgo -u "$target_ip":80 -w /usr/share/wordlists/dirb/common.txt -t 20 2>&1 | tee -a "$LOG_FILE"
    log_attack "Dirgo" "$target_ip"
    sleep $((RANDOM % 6 + 5))
}

run_wfuzz() {
    local target_domain=$1
    echo "Running WFuzz for domain $target_domain..." | tee -a "$LOG_FILE"
    wfuzz -c -z file,/usr/share/wordlists/dirb/common.txt --hc 404 "$target_domain/FUZZ" 2>&1 | tee -a "$LOG_FILE"
    log_attack "WFuzz" "$target_domain"
    sleep $((RANDOM % 6 + 5))
}

run_metasploit_scan() {
    local target_ip=$1
    echo "Running Metasploit auxiliary scanner for $target_ip..." | tee -a "$LOG_FILE"
    msfconsole -q -x "use auxiliary/scanner/portscan/tcp; set RHOSTS $target_ip; run; exit" 2>&1 | tee -a "$LOG_FILE"
    log_attack "Metasploit Scan" "$target_ip"
    sleep $((RANDOM % 6 + 5))
}

run_nikto() {
    local target_domain=$1
    echo "Running Nikto for domain $target_domain..." | tee -a "$LOG_FILE"
    echo "n" | nikto -h "$target_domain" -nointeractive 2>&1 | tee -a "$LOG_FILE"
    log_attack "Nikto" "$target_domain"
    sleep $((RANDOM % 6 + 5))
}

# Main function to run reconnaissance tools at random intervals
main() {
    local target_ips=("192.168.68.80" "192.168.68.71" "192.168.68.77") 
    local target_domains=("http://192.168.68.80" "http://192.168.68.71" "http://192.168.68.77")

    echo "Running reconnaissance attacks on ${target_ips[*]}..." | tee -a "$LOG_FILE"

    tools=("run_nmap" "run_gobuster" "run_masscan" "run_dirgo" "run_wfuzz" "run_metasploit_scan" "run_nikto")

    for i in "${!target_ips[@]}"; do
        (
            local target_ip="${target_ips[$i]}"
            local target_domain="${target_domains[$i]}"

            while true; do
                tool=${tools[$RANDOM % ${#tools[@]}]}
                $tool "$target_ip" "$target_domain"
                random_delay=$((RANDOM % 81 + 100))
                echo "[$target_ip] Waiting for $random_delay seconds before running another attack..." | tee -a "$LOG_FILE"
                sleep $random_delay
            done
        ) & 
    done

    wait 
}

# Run the main function
main
