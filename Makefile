# Tauri App
run-tauri:
	@cd src-tauri && cargo test
	@npm run tauri dev

test-tauri:
	@cd src-tauri && cargo test

fmt:
	@cd src-tauri && cargo fmt
	@cd src-tauri && cargo test

clippy:
	@cd src-tauri && cargo clippy

# Python ML Module
run-ml:
	@ml/_venv/bin/python3 ml/main.py

build-exec:
	@cd ml && pyinstaller --onefile --add-data "models/ml/recondet_model.joblib:models/ml" --hidden-import sklearn --hidden-import sklearn.ensemble._forest --hidden-import numpy --hidden-import scipy main.py
	@rm -rf ml/build ml/dist ml/main.spec
