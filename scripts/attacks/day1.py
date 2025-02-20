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
    gobuster_cmd = f"gobuster dir -u http://{target_domain} -w /path/to/wordlist.txt"
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
    sublist3r_cmd = f"sublist3r -d {target_domain} -o subdomains.txt"
    print(f"Running Sublist3r for {target_domain}...")
    os.system(sublist3r_cmd)
    time.sleep(random.randint(5, 10))

# Function to run TheHarvester (email and domain enumeration)
def run_theharvester(target_domain):
    theharvester_cmd = f"theharvester -d {target_domain} -b google -l 100"
    print(f"Running TheHarvester for {target_domain}...")
    os.system(theharvester_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Amass (subdomain enumeration and OSINT)
def run_amass(target_domain):
    amass_cmd = f"amass enum -d {target_domain} -o amass_output.txt"
    print(f"Running Amass for {target_domain}...")
    os.system(amass_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Nikto (web vulnerability scanner)
def run_nikto(target_ip):
    nikto_cmd = f"nikto -h {target_ip}"
    print(f"Running Nikto for {target_ip}...")
    os.system(nikto_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Shodan (OSINT & IoT search engine)
def run_shodan(target_ip):
    shodan_cmd = f"shodan host {target_ip}"
    print(f"Running Shodan for {target_ip}...")
    os.system(shodan_cmd)
    time.sleep(random.randint(5, 10))

# Main function to run reconnaissance tools at random intervals
def main():
    target_ip = "192.168.1.101"  # Replace with your target IP for Nmap, Nikto, Shodan
    target_domain = "example.com"  # Replace with your target domain for subdomain enumeration, etc.

    # Run reconnaissance tools in an infinite loop with random intervals
    print(f"Running reconnaissance attacks on {target_domain} and {target_ip}...")
    tools = [
        run_nmap,
        run_gobuster,
        run_recon_ng,
        run_sublist3r,
        run_theharvester,
        run_amass,
        run_nikto,
        run_shodan
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
        elif tool == run_theharvester:
            tool(target_domain)
        elif tool == run_amass:
            tool(target_domain)
        elif tool == run_nikto:
            tool(target_ip)
        elif tool == run_shodan:
            tool(target_ip)

        # Random delay between 10 to 30 minutes (600 to 1800 seconds)
        random_delay = random.randint(600, 1800)
        print(f"Waiting for {random_delay} seconds before running another attack...")
        time.sleep(random_delay)

if __name__ == "__main__":
    main()
