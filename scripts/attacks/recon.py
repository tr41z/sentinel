import os
import time
import random
from datetime import datetime

# Function to run Nmap with various random flags for reconnaissance
def run_nmap(target_ip):
    nmap_flags = [
        "-sS",  # SYN scan
        "-sU",  # UDP scan
        "-p-",   # Scan all ports
        "-A",   # Aggressive scan (OS detection, version detection, script scanning)
        "-O",   # OS detection
        "-sV",  # Version detection
        "-T4",  # Faster scan
    ]
    selected_flags = random.sample(nmap_flags, k=random.randint(1, len(nmap_flags)))
    nmap_cmd = f"nmap {' '.join(selected_flags)} {target_ip}"
    print(f"Running Nmap with flags: {' '.join(selected_flags)}")
    os.system(nmap_cmd)
    time.sleep(random.randint(5, 10))  # Random delay between scans

# Function to run Gobuster (subdomain brute force) with random wordlists
def run_gobuster(target_domain):
    wordlists = [
        "/usr/share/wordlists/dirb/common.txt",
        "/usr/share/wordlists/dirb/big.txt",
        "/usr/share/wordlists/dirb/medium.txt",
    ]
    selected_wordlist = random.choice(wordlists)
    gobuster_cmd = f"gobuster dir -u http://{target_domain} -w {selected_wordlist}"
    print(f"Running Gobuster for domain {target_domain} with wordlist {selected_wordlist}...")
    os.system(gobuster_cmd)
    time.sleep(random.randint(5, 10))

# Function to run other reconnaissance tools like Recon-ng with random modules
def run_recon_ng(target_domain):
    recon_modules = [
        "whois",
        "subdomains",
        "dns",
        "github",
        "shodan",
    ]
    selected_module = random.choice(recon_modules)
    recon_cmd = f"recon-ng -t {target_domain} -m {selected_module}"
    print(f"Running Recon-ng for {target_domain} using module {selected_module}...")
    os.system(recon_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Sublist3r (subdomain enumeration) with random options
def run_sublist3r(target_domain):
    sublist3r_flags = [
        "-d",  # Domain to scan
        "-p",  # Specify ports to scan
        "-b",  # Use Bing search engine
        "-e",  # Use different enumeration engines
    ]
    selected_flag = random.choice(sublist3r_flags)
    sublist3r_cmd = f"sublist3r {selected_flag} {target_domain}"
    print(f"Running Sublist3r for {target_domain} with flag {selected_flag}...")
    os.system(sublist3r_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Amass (subdomain enumeration and OSINT) with random parameters
def run_amass(target_domain):
    amass_flags = [
        "enum",  # Enumerate subdomains
        "intel", # Intelligence gathering
        "brute", # Brute force mode
    ]
    selected_flag = random.choice(amass_flags)
    amass_cmd = f"amass {selected_flag} -d {target_domain}"
    print(f"Running Amass for {target_domain} with mode {selected_flag}...")
    os.system(amass_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Nikto (web vulnerability scanner) with random parameters
def run_nikto(target_ip):
    nikto_flags = [
        "-h",  # Target host
        "-Tuning",  # Scan tuning options
        "-Format",  # Output format
    ]
    selected_flag = random.choice(nikto_flags)
    nikto_cmd = f"nikto {selected_flag} {target_ip}"
    print(f"Running Nikto for {target_ip} with flag {selected_flag}...")
    os.system(nikto_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Shodan (OSINT & IoT search engine) with random queries
def run_shodan(target_ip):
    shodan_queries = [
        "host",  # Query for host information
        "count",  # Query for count of matching results
        "myip",  # Show the current public IP
    ]
    selected_query = random.choice(shodan_queries)
    shodan_cmd = f"shodan {selected_query} {target_ip}"
    print(f"Running Shodan for {target_ip} with query {selected_query}...")
    os.system(shodan_cmd)
    time.sleep(random.randint(5, 10))
    
# Function to run Masscan (fast port scanning) with random rate
def run_masscan(target_ip):
    masscan_rates = [
        "--rate=1000",
        "--rate=5000",
        "--rate=10000",
    ]
    selected_rate = random.choice(masscan_rates)
    masscan_cmd = f"masscan {target_ip} -p0-65535 {selected_rate}"
    print(f"Running Masscan for {target_ip} with rate {selected_rate}...")
    os.system(masscan_cmd)
    time.sleep(random.randint(5, 10))

# Function to run WhatWeb (web application fingerprinting) with random flags
def run_whatweb(target_ip):
    whatweb_flags = [
        "--follow-redirects",
        "--no-banner",
        "--color=never",
    ]
    selected_flag = random.choice(whatweb_flags)
    whatweb_cmd = f"whatweb {selected_flag} {target_ip}"
    print(f"Running WhatWeb for {target_ip} with flag {selected_flag}...")
    os.system(whatweb_cmd)
    time.sleep(random.randint(5, 10))

# Main function to run reconnaissance tools at random intervals
def main():
    target_ip = "192.168.1.102"  
    target_domain = "http://192.168.1.102:80"  

    # Run reconnaissance tools in an infinite loop with random intervals
    print(f"Running reconnaissance attacks on {target_domain} and {target_ip}...")
    tools = [
        run_nmap,
        run_gobuster,
        run_recon_ng,
        run_sublist3r,
        run_amass,
        run_nikto,
        run_shodan,
        run_masscan,  
        run_whatweb,   
    ]

    while True:
        # Randomly select a tool to run
        tool = random.choice(tools)

        # Run the selected tool
        if tool == run_nmap:
            tool(target_ip)
        elif tool == run_gobuster:
            tool(target_domain)
        elif tool == run_recon_ng:
            tool(target_domain)
        elif tool == run_sublist3r:
            tool(target_domain)
        elif tool == run_amass:
            tool(target_domain)
        elif tool == run_nikto:
            tool(target_ip)
        elif tool == run_shodan:
            tool(target_ip)
        elif tool == run_masscan:
            tool(target_ip) 
        elif tool == run_whatweb:
            tool(target_ip)  
            
        # Random delay between 10 to 30 minutes (600 to 1800 seconds)
        random_delay = random.randint(600, 1800)
        print(f"Waiting for {random_delay} seconds before running another attack...")
        time.sleep(random_delay)

if __name__ == "__main__":
    main()
