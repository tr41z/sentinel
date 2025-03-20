import logging
import joblib
import numpy as np
import pandas as pd
import os
import sys
from config import CERTAINTY_THRESHOLD

# Configure logging
logging.basicConfig(
    filename="ai_module.log",
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s"
)

# Define paths
if getattr(sys, 'frozen', False):
    bundle_dir = sys._MEIPASS
else:
    bundle_dir = os.path.dirname(os.path.abspath(__file__))

MODEL_PATH = os.path.join(bundle_dir, 'models', 'recondos_model_gnb.joblib')

# Load model
try:
    logging.info("Loading model from %s", MODEL_PATH)
    model = joblib.load(MODEL_PATH)
except Exception as e:
    logging.error("Failed to load model: %s", e)
    raise

# Define feature columns
FEATURE_COLUMNS = [
    "protocol", "total_bytes", "rate", "avg_packet_size", "total_packet_count",
    "src_system_ports_count", "src_registered_ports_count", "src_dynamic_ports_count",
    "dst_system_ports_count", "dst_registered_ports_count", "dst_dynamic_ports_count",
    "packets_per_sec", "is_brute_target", "is_dos_target", "duration"
]

def predict_flows(df):
    """Predict malicious traffic using the trained model without scaling."""
    if df.empty:
        logging.warning("Received empty DataFrame for prediction")
        return [], []

    try:
        logging.info("Extracting features for prediction")
        features = df[FEATURE_COLUMNS].copy()
    except KeyError as e:
        logging.error("Missing expected feature columns: %s", e)
        raise ValueError(f"Missing expected feature columns: {e}")

    # Extract flow IDs
    flow_ids = df["id"].tolist()

    try:
        logging.info("Making predictions on %d flows", len(flow_ids))
        probabilities = model.predict_proba(features.to_numpy())
    except Exception as e:
        logging.error("Error during prediction: %s", e)
        raise

    predictions = []
    flagged = []

    for flow_id, probs in zip(flow_ids, probabilities):
        prediction = np.argmax(probs)
        certainty = np.max(probs)

        predictions.append((flow_id, prediction, certainty))

        if prediction == 1 and certainty > CERTAINTY_THRESHOLD:
            flagged.append((flow_id, prediction, certainty))

    logging.info("Predictions completed: %d total, %d flagged", len(predictions), len(flagged))

    return predictions, flagged
