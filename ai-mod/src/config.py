import os

HOME_DIR = os.path.expanduser("~")
DB_PATH = os.path.join(HOME_DIR, ".sentinel", "sentinel.db")

BATCH_SIZE = 100
PREDICTION_INTERVAL = 60
CERTAINTY_THRESHOLD = 0.51