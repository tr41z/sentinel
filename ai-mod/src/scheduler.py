import time
import threading
from database import fetch_unprocessed_flows, insert_predictions, insert_flagged_flows, update_flagged_ips
from model import predict_flows
from config import PREDICTION_INTERVAL
import logging

def process_new_flows():
    while True:
        df = fetch_unprocessed_flows()
        predictions, flagged = predict_flows(df)

        if predictions:
            insert_predictions(predictions)

        if flagged:
            insert_flagged_flows(flagged)

            logging.info("Updating flagged_ips inside scheduler")
            update_flagged_ips()  # Automatically update flagged IPs in the background

        time.sleep(PREDICTION_INTERVAL)

def start_scheduler():
    thread = threading.Thread(target=process_new_flows, daemon=True)
    thread.start()