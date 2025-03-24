import pytest
import pandas as pd
from model import predict_flows

@pytest.fixture
def sample_flow_data():
    return pd.DataFrame({
        "id": [1, 2],
        "protocol": [6, 17],
        "total_bytes": [500, 1000],
        "rate": [1.5, 2.0],
        "avg_packet_size": [100, 200],
        "total_packet_count": [5, 10],
        "src_system_ports_count": [1, 2],
        "src_registered_ports_count": [1, 1],
        "src_dynamic_ports_count": [3, 4],
        "dst_system_ports_count": [2, 3],
        "dst_registered_ports_count": [1, 2],
        "dst_dynamic_ports_count": [4, 5],
        "packets_per_sec": [0.5, 1.0]
    })

def test_predict_flows(sample_flow_data, mocker):
    mock_model = mocker.Mock()
    mock_model.predict_proba.return_value = [[0.2, 0.8], [0.9, 0.1]]

    mocker.patch("model.model_pipeline.predict_proba", mock_model.predict_proba)

    predictions, flagged = predict_flows(sample_flow_data)

    assert len(predictions) == 2
    assert predictions[0][1] == 1
    assert len(flagged) == 1  # Only first flow is flagged
