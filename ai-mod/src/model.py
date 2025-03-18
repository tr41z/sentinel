import joblib
import numpy as np
import pandas as pd
from config import CERTAINTY_THRESHOLD

# Load the trained model and the scaler
MODEL_PATH = "models/recondos_model_lr.joblib"
SCALER_PATH = "models/scaler.joblib" 

model = joblib.load(MODEL_PATH)
scaler = joblib.load(SCALER_PATH)  # Load the scaler

FEATURE_COLUMNS = [
    "protocol", "total_bytes", "rate", "avg_packet_size", "total_packet_count",
    "src_system_ports_count", "src_registered_ports_count", "src_dynamic_ports_count",
    "dst_system_ports_count", "dst_registered_ports_count", "dst_dynamic_ports_count",
    "packets_per_sec", "is_brute_target", "is_dos_target", "duration"
]

def predict_flows(df):
    if df.empty:
        return [], []

    try:
        features = df[FEATURE_COLUMNS]
    except KeyError as e:
        raise ValueError(f"Missing expected feature columns: {e}")

    # Convert the dataframe to match the structure the scaler expects
    features_scaled = scaler.transform(features)  # scale using the same scaler that was used during training

    flow_ids = df["id"].tolist()
    probabilities = model.predict_proba(features_scaled)

    predictions = []
    flagged = []

    for flow_id, probs in zip(flow_ids, probabilities):
        prediction = np.argmax(probs)
        certainty = np.max(probs)

        # Append to predictions list for all flows
        predictions.append((flow_id, prediction, certainty))

        # Only flag the flow if it's predicted as malicious (prediction == 1) and certainty > threshold
        if prediction == 1 and certainty > CERTAINTY_THRESHOLD:
            flagged.append((flow_id, prediction, certainty))

    return predictions, flagged

