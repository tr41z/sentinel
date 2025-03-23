#!/bin/bash

# Log file with timestamp
LOG_FILE="recon_log_$(date '+%Y%m%d').txt"

# Function to log attack details
log_attack() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - Executed $1 on $2" >> "$LOG_FILE"
}

# Function to run Nmap with varied flags
run_nmap() {
    local target_ip=$1
    nmap_flags=("-sS" "-sU" "-p-" "-A" "-O" "-sV" "-T4" "-Pn" "-F" "-sC" "--script=vuln" "--script=http-title")

    # Select 3-5 random flags
    selected_flags=()
    num_flags=$((RANDOM % 3 + 3))
    
    while [ ${#selected_flags[@]} -lt $num_flags ]; do
        flag=${nmap_flags[RANDOM % ${#nmap_flags[@]}]}
        [[ " ${selected_flags[@]} " =~ " $flag " ]] || selected_flags+=("$flag")
    done

    nmap "${selected_flags[@]}" "$target_ip" &> /dev/null
    log_attack "Nmap (${selected_flags[*]})" "$target_ip"
    sleep $((RANDOM % 10 + 5))
}

# Function to run Gobuster for directory scanning
run_gobuster() {
    local target_domain=$1
    gobuster dir -u "$target_domain" -w /usr/share/wordlists/dirb/common.txt &> /dev/null
    log_attack "Gobuster" "$target_domain"
    sleep $((RANDOM % 10 + 5))
}

# Function to run Masscan for fast scanning
run_masscan() {
    local target_ip=$1
    masscan "$target_ip" -p0-65535 --rate=5000 &> /dev/null
    log_attack "Masscan" "$target_ip"
    sleep $((RANDOM % 10 + 5))
}

# Function to run WFuzz for web fuzzing
run_wfuzz() {
    local target_domain=$1
    wfuzz -c -z file,/usr/share/wordlists/dirb/common.txt --hc 404 "$target_domain/FUZZ" &> /dev/null
    log_attack "WFuzz" "$target_domain"
    sleep $((RANDOM % 10 + 5))
}

# Function to run Metasploit TCP scanner
run_metasploit_scan() {
    local target_ip=$1
    msfconsole -q -x "use auxiliary/scanner/portscan/tcp; set RHOSTS $target_ip; run; exit" &> /dev/null
    log_attack "Metasploit Scan" "$target_ip"
    sleep $((RANDOM % 10 + 5))
}

# Function to run Nikto web vulnerability scanner
run_nikto() {
    local target_domain=$1
    echo "n" | nikto -h "$target_domain" -nointeractive &> /dev/null
    log_attack "Nikto" "$target_domain"
    sleep $((RANDOM % 10 + 5))
}

# Function to run Dnsrecon for DNS enumeration
run_dnsrecon() {
    local target_domain=$1
    dnsrecon -d "$target_domain" &> /dev/null
    log_attack "Dnsrecon" "$target_domain"
    sleep $((RANDOM % 10 + 5))
}

# Function to run WhatWeb for fingerprinting web technologies
run_whatweb() {
    local target_domain=$1
    whatweb "$target_domain" &> /dev/null
    log_attack "WhatWeb" "$target_domain"
    sleep $((RANDOM % 10 + 5))
}

# Function to run Amass for subdomain enumeration
run_amass() {
    local target_domain=$1
    amass enum -d "$target_domain" &> /dev/null
    log_attack "Amass" "$target_domain"
    sleep $((RANDOM % 10 + 5))
}

# Function to run TheHarvester for OSINT gathering
run_theharvester() {
    local target_domain=$1
    theHarvester -d "$target_domain" -b all &> /dev/null
    log_attack "TheHarvester" "$target_domain"
    sleep $((RANDOM % 10 + 5))
}

# Function to run Katana for fast web crawling
run_katana() {
    local target_domain=$1
    katana -u "$target_domain" -jc -d 5 -o katana_output.txt &> /dev/null
    log_attack "Katana Web Crawler" "$target_domain"
    sleep $((RANDOM % 10 + 5))
}

# Main function to execute reconnaissance tools at random intervals
main() {
    local target_ips=("192.168.0.11" "192.168.0.18") 
    local target_domains=("http://192.168.0.11" "http://192.168.0.18")

    echo "Logging reconnaissance scans to $LOG_FILE"

    tools=("run_nmap" "run_gobuster" "run_masscan" "run_wfuzz" "run_metasploit_scan" "run_nikto" "run_dnsrecon" "run_whatweb" "run_amass" "run_theharvester" "run_katana")

    for i in "${!target_ips[@]}"; do
        (
            local target_ip="${target_ips[$i]}"
            local target_domain="${target_domains[$i]}"

            while true; do
                tool=${tools[$RANDOM % ${#tools[@]}]}
                $tool "$target_ip" "$target_domain"
                random_delay=$((RANDOM % 121 + 90))  # Random delay between 90-210 sec
                sleep $random_delay
            done
        ) & 
    done

    wait 
}

# Execute the main function
main
