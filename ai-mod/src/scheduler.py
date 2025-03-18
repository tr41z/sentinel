import time
import threading
from database import fetch_unprocessed_flows, insert_predictions, insert_flagged_flows
from model import predict_flows
from config import PREDICTION_INTERVAL

def process_new_flows():
    while True:
        df = fetch_unprocessed_flows()
        predictions, flagged = predict_flows(df)

        if predictions:
            insert_predictions(predictions)

        if flagged:
            insert_flagged_flows(flagged)

        time.sleep(PREDICTION_INTERVAL)

def start_scheduler():
    thread = threading.Thread(target=process_new_flows, daemon=True)
    thread.start()