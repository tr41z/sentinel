run:
	@cd src-tauri && cargo test
	@npm run tauri dev

test:
	@cd src-tauri && cargo test

fmt:
	@cd src-tauri && cargo fmt
	@cd src-tauri && cargo test

clippy:
	@cd src-tauri && cargo clippy