import logging
from fastapi import FastAPI
from scheduler import start_scheduler
from database import initialize_db

# Configure logging
logging.basicConfig(
    filename="ai_module.log",
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s"
)

logging.info("Initializing database")
initialize_db()

app = FastAPI()

@app.get("/")
def root():
    logging.info("Received request at /")
    return {"message": "Intrusion Detection API is running"}

@app.get("/predict")
def trigger_prediction():
    from database import fetch_unprocessed_flows, insert_predictions, insert_flagged_flows
    from model import predict_flows

    logging.info("Triggering AI predictions")
    try:
        df = fetch_unprocessed_flows()
        predictions, flagged = predict_flows(df)

        if predictions:
            logging.info("Inserting %d predictions into the database", len(predictions))
            insert_predictions(predictions)

        if flagged:
            logging.info("Inserting %d flagged flows into the database", len(flagged))
            insert_flagged_flows(flagged)

        return {"message": "Predictions completed", "predictions_made": len(predictions)}

    except Exception as e:
        logging.error("Error during prediction: %s", e)
        return {"message": "Error during prediction", "error": str(e)}

if __name__ == "__main__":
    import uvicorn
    logging.info("Starting scheduler and FastAPI server")
    start_scheduler()
    uvicorn.run(app, host="0.0.0.0", port=8000)
