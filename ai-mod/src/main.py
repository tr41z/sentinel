from fastapi import FastAPI
from scheduler import start_scheduler
from database import initialize_db

initialize_db()

app = FastAPI()

@app.get("/")
def root():
    return {"message": "Intrusion Detection API is running"}

@app.get("/predict")
def trigger_prediction():
    from database import fetch_unprocessed_flows, insert_predictions, insert_flagged_flows
    from model import predict_flows
    
    df = fetch_unprocessed_flows()
    predictions, flagged = predict_flows(df)

    if predictions:
        insert_predictions(predictions)

    if flagged:
        insert_flagged_flows(flagged)

    return {"message": "Predictions completed", "predictions_made": len(predictions)}

if __name__ == "__main__":
    import uvicorn
    start_scheduler()
    uvicorn.run(app, host="0.0.0.0", port=8000)
