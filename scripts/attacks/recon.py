import os
import time
import random
from datetime import datetime

# Function to run Nmap with various flags for reconnaissance
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
    for flag in nmap_flags:
        print(f"Running Nmap with flag: {flag}")
        os.system(f"nmap {flag} {target_ip}")
        time.sleep(random.randint(5, 10))  # Random delay between scans

# Function to run Gobuster (subdomain brute force)
def run_gobuster(target_domain):
    gobuster_cmd = f"gobuster dir -u {target_domain} -w /usr/share/wordlists/dirb/common.txt"
    print(f"Running Gobuster for domain {target_domain}...")
    os.system(gobuster_cmd)
    time.sleep(random.randint(5, 10))

# Function to run other reconnaissance tools like Recon-ng
def run_recon_ng(target_domain):
    recon_cmd = f"recon-ng -t {target_domain}"
    print(f"Running Recon-ng for {target_domain}...")
    os.system(recon_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Sublist3r (subdomain enumeration)
def run_sublist3r(target_domain):
    sublist3r_cmd = f"sublist3r -d {target_domain}"
    print(f"Running Sublist3r for {target_domain}...")
    os.system(sublist3r_cmd)
    time.sleep(random.randint(5, 10))


# Function to run Amass (subdomain enumeration and OSINT)
def run_amass(target_domain):
    amass_cmd = f"amass enum -d {target_domain}"
    print(f"Running Amass for {target_domain}...")
    os.system(amass_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Nikto (web vulnerability scanner)
def run_nikto(target_ip):
    nikto_cmd = f"nikto -h {target_ip}"
    print(f"Running Nikto for {target_ip}...")
    os.system(nikto_cmd)
    time.sleep(random.randint(5, 10))
    
# Function to run Masscan (fast port scanning)
def run_masscan(target_ip):
    masscan_cmd = f"masscan {target_ip} -p0-65535 --rate=1000"
    print(f"Running Masscan for {target_ip}...")
    os.system(masscan_cmd)
    time.sleep(random.randint(5, 10))

# Function to run WhatWeb (web application fingerprinting)
def run_whatweb(target_ip):
    whatweb_cmd = f"whatweb {target_ip}"
    print(f"Running WhatWeb for {target_ip}...")
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
        elif tool == run_masscan:
            tool(target_ip)  
        elif tool == run_whatweb:
            tool(target_ip) 

        # Random delay between 10 to 30 minutes (600 to 1800 seconds)
        random_delay = random.randint(120, 600)
        print(f"Waiting for {random_delay} seconds before running another attack...")
        time.sleep(random_delay)

if __name__ == "__main__":
    main()