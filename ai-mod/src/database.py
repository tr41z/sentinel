import sqlite3
import pandas as pd
from config import DB_PATH, BATCH_SIZE
import logging

def initialize_db():
    """Ensure that required tables exist in the database."""
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()

    # Create predictions table if it doesn't exist
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS predictions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            flow_id INTEGER UNIQUE,
            prediction INTEGER,
            certainty REAL, 
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    ''')

    # Create flagged_flows table if it doesn't exist
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS flagged_flows (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            flow_id INTEGER UNIQUE,
            prediction INTEGER,
            certainty REAL, 
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    ''')

    # Create flagged_ips table with unique src_ip and last_seen timestamp
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS flagged_ips (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            src_ip TEXT UNIQUE,
            first_seen DATETIME DEFAULT CURRENT_TIMESTAMP,
            last_seen DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    ''')

    conn.commit()
    conn.close()

def get_db_connection():
    return sqlite3.connect(DB_PATH, check_same_thread=False)

def fetch_unprocessed_flows():
    conn = get_db_connection()
    query = f"""
    SELECT id, protocol, total_bytes, rate, avg_packet_size, total_packet_count,
           src_system_ports_count, src_registered_ports_count, src_dynamic_ports_count,
           dst_system_ports_count, dst_registered_ports_count, dst_dynamic_ports_count,
           packets_per_sec, is_brute_target, is_dos_target, duration
    FROM flows
    WHERE id NOT IN (SELECT flow_id FROM predictions)
    LIMIT {BATCH_SIZE}
    """
    df = pd.read_sql(query, conn)
    conn.close()
    return df

def insert_predictions(predictions):
    """Insert predictions into the database."""
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()

    cursor.executemany("""
        INSERT INTO predictions (flow_id, prediction, certainty) VALUES (?, ?, ?)
    """, [(flow_id, int(prediction), float(certainty)) for flow_id, prediction, certainty in predictions])

    conn.commit()
    conn.close()

def insert_flagged_flows(flagged):
    conn = get_db_connection()
    cursor = conn.cursor()

    cursor.executemany("""
        INSERT INTO flagged_flows (flow_id, prediction, certainty) VALUES (?, ?, ?)
    """, [(flow_id, int(prediction), float(certainty)) for flow_id, prediction, certainty in flagged])

    conn.commit()
    conn.close()

def update_flagged_ips():
    conn = get_db_connection()
    cursor = conn.cursor()

    logging.info("Updating flagged_ips...")  # Log execution start

    # Check if flagged_flows has data
    cursor.execute("SELECT COUNT(*) FROM flagged_flows")
    flagged_count = cursor.fetchone()[0]
    logging.info(f"Flagged flows count: {flagged_count}")

    if flagged_count == 0:
        logging.info("No flagged flows found, skipping flagged_ips update.")
        conn.close()
        return

    # Insert new flagged IPs
    cursor.execute('''
        INSERT OR IGNORE INTO flagged_ips (src_ip, first_seen, last_seen)
        SELECT DISTINCT f.src_ip, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP
        FROM flagged_flows ff
        JOIN flows f ON ff.flow_id = f.id
    ''')

    # Update last_seen for already existing IPs
    cursor.execute('''
        UPDATE flagged_ips
        SET last_seen = CURRENT_TIMESTAMP
        WHERE src_ip IN (SELECT f.src_ip FROM flagged_flows ff JOIN flows f ON ff.flow_id = f.id)
    ''')

    rows_affected = cursor.rowcount
    conn.commit()
    conn.close()

    logging.info(f"Updated {rows_affected} rows in flagged_ips")  # Log completion