from fastapi.testclient import TestClient
from main import app

client = TestClient(app)

def test_root():
    response = client.get("/")
    assert response.status_code == 200
    assert response.json() == {"message": "Intrusion Detection API is running"}

def test_trigger_prediction(mocker):
    mocker.patch("database.fetch_unprocessed_flows", return_value=[])
    mocker.patch("model.predict_flows", return_value=([], []))
    
    response = client.get("/predict")
    assert response.status_code == 200
    assert response.json()["message"] == "Predictions completed"
