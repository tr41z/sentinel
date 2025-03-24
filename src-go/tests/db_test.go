package tests

import (
	"backend/db"
	"database/sql"
	"net/http"
	"net/http/httptest"
	"testing"

	_ "github.com/mattn/go-sqlite3"
)

func TestFetchFlows(t *testing.T) {
	db.DB, _ = sql.Open("sqlite3", ":memory:")
	defer db.DB.Close()

	// Create a mock flows table
	db.DB.Exec(`CREATE TABLE flows (
		id INTEGER PRIMARY KEY,
		src_ip TEXT,
		dst_ip TEXT,
		src_ports TEXT,
		dst_ports TEXT,
		protocol INTEGER,
		total_bytes INTEGER,
		rate REAL,
		avg_packet_size REAL,
		total_packet_count INTEGER,
		start_time INTEGER,
		last_updated_time INTEGER,
		duration INTEGER
	)`)
	db.DB.Exec(`INSERT INTO flows (src_ip, dst_ip, src_ports, dst_ports, protocol, total_bytes, rate, avg_packet_size, total_packet_count, start_time, last_updated_time, duration)
		VALUES ('192.168.1.1', '192.168.1.2', '80', '443', 6, 1000, 10.5, 50.0, 20, 1620000000, 1620003600, 3600)`)

	req := httptest.NewRequest("GET", "/api/v1/flows", nil)
	w := httptest.NewRecorder()

	db.FetchFlows(w, req)

	if w.Code != http.StatusOK {
		t.Errorf("Expected status 200, got %d", w.Code)
	}
}
