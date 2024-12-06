import asyncio
import os
import signal
import sys
import time
import traceback

import database.db as db
import pandas as pd
from joblib import load
from models.input import CLFModelParams
from sklearn.cluster import DBSCAN
from sklearn.preprocessing import MinMaxScaler

# Loading ML model from file
if hasattr(sys, "_MEIPASS"):  # Check if running in a PyInstaller bundle
    model_path = os.path.join(sys._MEIPASS, "models/ml/recondet_model.joblib")
else:
    model_path = "models/ml/recondet_model.joblib"  # Local path when running normally

try:
    clf = load(model_path)
    print("Model loaded successfully!")
except Exception as e:
    print(f"Error loading model: {e}")
    CLF = None  # Set clf to None if loading fails

BATCH_SIZE = 100  # Batch size for DBSCAN
WEIGHT_FACTOR = 100
DBSCAN_EPS = 0.001


async def process_flows():
    """Retrieve unprocessed flows in batches, cluster with DBSCAN, and classify densest cluster."""
    flows = db.get_unprocessed_flows()

    if not flows:
        print("No new unprocessed flows to process.")
        return

    # Process flows in batches of 300
    for i in range(0, len(flows), BATCH_SIZE):
        batch_flows = flows[i : i + BATCH_SIZE]

        # Convert flows into a DataFrame
        df = pd.DataFrame(
            batch_flows,
            columns=[
                "id",
                "protocol",
                "total_bytes",
                "total_packet_count",
                "sbytes",
                "smean",
                "dmean",
                "dbytes",
                "dload",
                "sload",
                "dpkts",
                "rate",
                "dttl",
                "spkts",
                "start_time",
                "src_ip",
            ],
        )

        # Normalize selected features for DBSCAN
        features = [
            "protocol",
            "total_bytes",
            "total_packet_count",
            "sbytes",
            "smean",
            "dmean",
            "dbytes",
            "dload",
            "sload",
            "dpkts",
            "rate",
            "dttl",
            "spkts",
            "start_time",
        ]
        scaler = MinMaxScaler()
        normalized_data = scaler.fit_transform(df[features])

        # Cluster with DBSCAN
        clustering = DBSCAN(eps=DBSCAN_EPS, min_samples=30).fit(normalized_data)
        df["cluster"] = clustering.labels_

        # Print clustering results
        print(f"\nDBSCAN Clustering Results:")
        print(f"Unique clusters found: {df['cluster'].unique()}")
        print(f"Cluster counts:\n{df['cluster'].value_counts()}\n")

        # Find the densest cluster
        if len(df["cluster"].value_counts()) == 0:
            print("No clusters found in this batch.")
            continue

        # Find the densest cluster, ignoring noise (-1)
        valid_clusters = df["cluster"].value_counts()
        if -1 in valid_clusters.index:
            del valid_clusters[-1]  # Remove noise from valid clusters

        if valid_clusters.empty:
            print("No valid clusters found in this batch.")
            continue

        # Find the densest cluster
        densest_cluster = df["cluster"].value_counts().idxmax()
        dense_cluster_flows = df[df["cluster"] == densest_cluster]

        # Classify flows in the densest cluster
        for _, flow in dense_cluster_flows.iterrows():
            # Extract features for the classifier
            clf_params = CLFModelParams(
                sbytes=flow["sbytes"],
                smean=flow["smean"],
                dmean=flow["dmean"],
                dbytes=flow["dbytes"],
                dload=flow["dload"],
                sload=flow["sload"],
                dpkts=flow["dpkts"],
                rate=flow["rate"],
                dttl=flow["dttl"],
                spkts=flow["spkts"],
            )
            clf_df = pd.DataFrame([clf_params.model_dump()])

            # Print the features being passed to the classifier
            print(f"\nPassing to classifier:\n{clf_df}")

            # Predict malicious probability
            malicious_prob = clf.predict_proba(clf_df)[0][1]
            benign_prob = clf.predict_proba(clf_df)[0][0]

            prediction = "Malicious" if malicious_prob >= 0.9 else "Benign"

            if prediction == "Malicious":
                # Flag the IP if it's malicious
                await db.flag_ip_if_malicious(flow["src_ip"])

            # Insert prediction into database
            new_flow_id = db.insert_prediction(prediction, malicious_prob, benign_prob)

            if new_flow_id is not None:
                print(
                    f"Processed flow with prediction: {prediction}, flow_id: {new_flow_id}"
                )
            else:
                print("Failed to insert prediction into the database.")


async def periodic_flow_processing():
    """Runs every 20 seconds to process new flows."""
    while True:
        print("Processing new flows...")
        await process_flows()  # Await the async process_flows
        await asyncio.sleep(60)  # Wait for 60 seconds


def shutdown(signum, frame):
    print(f"Received signal {signum}. Shutting down gracefully...")
    db.close_connection()
    sys.exit(0)


if __name__ == "__main__":
    # Register the signal handler for graceful shutdown
    signal.signal(signal.SIGINT, shutdown)
    signal.signal(signal.SIGTERM, shutdown)

    # Create necessary tables at the start
    db.create_predictions_table()
    db.create_flagged_ips_table()

    # time.sleep(120)
    # Start the periodic flow processing in the background
    try:
        asyncio.run(periodic_flow_processing())  # Runs the background task
    except Exception as e:
        print(f"Error in main loop: {e}")
        traceback.print_exc()  # Print the traceback for debugging
    finally:
        db.close_connection()
