import sqlite3
import pytest
import pandas as pd
from database import initialize_db, fetch_unprocessed_flows, insert_predictions, insert_flagged_flows, update_flagged_ips, get_db_connection

@pytest.fixture(scope="module")
def setup_database():
    initialize_db()
    conn = get_db_connection()
    yield conn
    conn.close()

def test_initialize_db(setup_database):
    conn = setup_database
    cursor = conn.cursor()
    cursor.execute("SELECT name FROM sqlite_master WHERE type='table'")
    tables = {row[0] for row in cursor.fetchall()}
    expected_tables = {"predictions", "flagged_flows", "flagged_ips"}
    assert expected_tables.issubset(tables)

def test_insert_predictions(setup_database):
    conn = setup_database
    insert_predictions([(1, 1, 0.95)])
    df = pd.read_sql("SELECT * FROM predictions", conn)
    assert not df.empty
    assert df.iloc[0]["flow_id"] == 1
    assert df.iloc[0]["prediction"] == 1

def test_insert_flagged_flows(setup_database):
    conn = setup_database
    cursor = conn.cursor()

    # Debugging: Check current flagged_flows data
    df_before = pd.read_sql("SELECT * FROM flagged_flows", conn)
    print("Before insert:", df_before)

    # Ensure no duplicate flow_id before inserting
    cursor.execute("DELETE FROM flagged_flows WHERE flow_id=2")
    conn.commit()

    insert_flagged_flows([(2, 1, 0.97)])

    df = pd.read_sql("SELECT * FROM flagged_flows", conn)
    print("After insert:", df)  # Debugging

    assert not df.empty
    assert df.iloc[0]["flow_id"] == 2, f"Expected flow_id=2, but got {df.iloc[0]['flow_id']}"

def test_update_flagged_ips(setup_database):
    conn = setup_database
    cursor = conn.cursor()

    # Ensure database is not locked by committing before insertion
    cursor.execute("DELETE FROM flows WHERE id=2")
    conn.commit()

    cursor.execute("INSERT INTO flows (id, src_ip) VALUES (2, '192.168.1.10')")
    conn.commit()
    
    conn.close()  # Close to prevent locking
    conn = get_db_connection()  # Reopen connection

    update_flagged_ips()

    df = pd.read_sql("SELECT * FROM flagged_ips WHERE src_ip='192.168.1.10'", conn)
    assert not df.empty
