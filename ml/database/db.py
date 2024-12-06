import os
import sqlite3
from dotenv import load_dotenv

# Load environment variables
load_dotenv()

# Define the database path
DB_PATH = os.path.expanduser("~/.sentinel/app_data.db")

# Establish database connection
_conn = None  # Singleton connection instance

def create_connection():
    """Establish or return the existing database connection."""
    global _conn
    if _conn is None:
        _conn = sqlite3.connect(DB_PATH)
        _conn.row_factory = sqlite3.Row  # To access columns by name
        print("Database connected successfully!")
    return _conn

def close_connection():
    """Close the database connection."""
    global _conn
    if _conn:
        _conn.close()
        _conn = None
        print("Database connection closed.")

def create_predictions_table():
    """Create the flows_predictions table to store prediction results."""
    conn = create_connection()
    cursor = conn.cursor()
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS flows_predictions (
            flow_id INTEGER PRIMARY KEY AUTOINCREMENT,
            prediction TEXT NOT NULL,
            malicious_prob REAL NOT NULL,
            benign_prob REAL NOT NULL
        )
    """)
    conn.commit()
    print("Table 'flows_predictions' created or already exists.")

def create_flagged_ips_table():
    """Create the flagged_ips table to store flagged IP addresses."""
    conn = create_connection()
    cursor = conn.cursor()
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS flagged_ips (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            flagged_ip TEXT NOT NULL UNIQUE
        )
    """)
    conn.commit()
    print("Table 'flagged_ips' created or already exists.")

def get_unprocessed_flows():
    """Retrieve all unprocessed flows from the 'flows' table."""
    conn = create_connection()
    cursor = conn.cursor()
    
    try:
        cursor.execute("""
            SELECT id, protocol, total_bytes, total_packet_count, sbytes, smean, dmean, 
                dbytes, dload, sload, dpkts, rate, dttl, spkts, start_time, src_ip
            FROM flows
        """)
        all_flows = cursor.fetchall()
        print(f"Total flows in 'flows' table: {len(all_flows)}")

        cursor.execute("""
            SELECT flow_id FROM flows_predictions
        """)
        processed_flows = cursor.fetchall()
        print(f"Total processed flows: {len(processed_flows)}")

        cursor.execute("""
            SELECT id, protocol, total_bytes, total_packet_count, sbytes, smean, dmean, 
                dbytes, dload, sload, dpkts, rate, dttl, spkts, start_time, src_ip
            FROM flows
            WHERE id NOT IN (SELECT flow_id FROM flows_predictions)
        """)
        flows = cursor.fetchall()
        print(f"Unprocessed flows retrieved successfully! Count: {len(flows)}")
        return flows

    except Exception as e:
        print(f"Error retrieving unprocessed flows: {e}")
        return []

def insert_prediction(prediction, malicious_prob, benign_prob):
    """Insert the prediction result into the database."""
    conn = create_connection()
    cursor = conn.cursor()
    
    try:
        insert_query = """
            INSERT INTO flows_predictions (prediction, malicious_prob, benign_prob) 
            VALUES (?, ?, ?)
        """
        cursor.execute(insert_query, (prediction, malicious_prob, benign_prob))
        conn.commit()
        flow_id = cursor.lastrowid
        print(f"Prediction inserted successfully with flow_id: {flow_id}")
        return flow_id
    except Exception as e:
        print(f"Error inserting prediction: {e}")
        return None  # Return None if insertion fails

async def flag_ip_if_malicious(ip):
    """Check if the IP is flagged as malicious and insert if not already flagged."""
    conn = create_connection()
    cursor = conn.cursor()
    
    try:
        # Check if the IP is already flagged
        cursor.execute("SELECT flagged_ip FROM flagged_ips WHERE flagged_ip = ?", (ip,))
        existing_ip = cursor.fetchone()

        if existing_ip is None:  # If IP is not flagged, insert it
            cursor.execute("INSERT INTO flagged_ips (flagged_ip) VALUES (?)", (ip,))
            conn.commit()
            print(f"IP flagged as malicious: {ip}")
        else:
            print(f"IP already flagged: {ip}")

    except Exception as e:
        print(f"Error flagging IP {ip}: {e}")

# Create the necessary tables after connecting to the database
create_predictions_table()
create_flagged_ips_table()