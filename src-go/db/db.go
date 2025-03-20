package db

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	_ "github.com/mattn/go-sqlite3"

	"backend/handlers"
	"backend/utils"
)

type Flow struct {
	ID                int     `json:"id"`
	SourceIp          string  `json:"source_ip"`
	DestinationIp     string  `json:"destination_ip"`
	SourcePorts       string  `json:"source_ports"`
	DestinationPorts  string  `json:"destination_ports"`
	Protocol          int8    `json:"protocol"`
	TotalBytes        int     `json:"total_bytes"`
	Rate              float64 `json:"rate"`
	AveragePacketSize float64 `json:"average_packet_size"`
	TotalPacketCount  int     `json:"total_packet_count"`
	StartTime         int     `json:"start_time"`
	LastUpdatedTime   int     `json:"last_updated_time"`
	Duration          int     `json:"duration"`
}

// Global variable for SQLite database connection
var DB *sql.DB

func InitDB() {
	dir := get_home_dir()
	fmt.Println(dir)
	var err error
	DB, err = sql.Open("sqlite3", dir)
	if err != nil {
		log.Fatal(err)
	}
}

func checkIfExpired(w http.ResponseWriter, r *http.Request) {
	row := DB.QueryRow("SELECT last_updated_time FROM flows LIMIT 3")
	now := time.Now()
	var lastUpdatedTime int64
	if err := row.Scan(&lastUpdatedTime); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	lastUpdated := time.Unix(lastUpdatedTime, 0)
	fmt.Println(now.Sub(lastUpdated).Seconds())

	// If first time diff between first row and now is larger than 1 day
	if now.Sub(lastUpdated).Seconds() >= 86400000 {
		handlers.StopSnifferHandler(w, r)
		/*
		 - Pause sniffer module
		 - Export statistics to another table (same db)
		 - Ensure it was saved
		 - Delete flows table
		 - Re-create flows table
		 - Resume sniffer module
		*/
	}
}

// TODO: Delete db and re-create db functions
func createTable() {}
func deleteTable() {}

// TODO: Save statistics to new table
func saveStatistics() {}

func FetchFlows(w http.ResponseWriter, r *http.Request) {
	// checkIfExpired(w, r)
	rows, err := DB.Query("SELECT id, src_ip, dst_ip, src_ports, " +
		"dst_ports, protocol, total_bytes, rate, " +
		"avg_packet_size, total_packet_count, start_time, last_updated_time, duration " +
		"FROM flows")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	defer rows.Close()

	flows := []Flow{} // Slice to store flows
	for rows.Next() {
		var flow Flow
		if err := rows.Scan(&flow.ID, &flow.SourceIp, &flow.DestinationIp, &flow.SourcePorts, &flow.DestinationPorts,
			&flow.Protocol, &flow.TotalBytes, &flow.Rate, &flow.AveragePacketSize, &flow.TotalPacketCount,
			&flow.StartTime, &flow.LastUpdatedTime, &flow.Duration); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
		flows = append(flows, flow)
	}

	// Set the response content type to JSON
	w.Header().Set("Content-Type", "application/json")

	// Write the JSON response
	if err := json.NewEncoder(w).Encode(flows); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func FetchThreats(w http.ResponseWriter, r *http.Request) {
	rows, err := DB.Query("SELECT id, flow_id, prediction, certainty, timestamp FROM predictions WHERE prediction = 1")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	defer rows.Close()

	var threats []map[string]interface{}
	for rows.Next() {
		var id, flowID int
		var prediction, certainty float64
		var timestamp time.Time // Change from int64 to time.Time

		if err := rows.Scan(&id, &flowID, &prediction, &certainty, &timestamp); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}

		threat := map[string]interface{}{
			"id":         id,
			"flow_id":    flowID,
			"prediction": prediction,
			"certainty":  certainty,
			"timestamp":  timestamp.Format(time.RFC3339), // Convert time.Time to string
		}
		threats = append(threats, threat)
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(threats)
}

func FetchFlaggedIPs(w http.ResponseWriter, r *http.Request) {
	rows, err := DB.Query("SELECT id, flow_id, prediction, certainty, timestamp FROM flagged_flows") // Specify needed columns
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	defer rows.Close()

	var flaggedFlows []map[string]interface{}
	for rows.Next() {
		var id, flowID int
		var prediction float64
		var certainty float64
		var timestamp time.Time // Assuming timestamp is stored as DATETIME

		if err := rows.Scan(&id, &flowID, &prediction, &certainty, &timestamp); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}

		flowData := map[string]interface{}{
			"id":         id,
			"flow_id":    flowID,
			"prediction": prediction,
			"certainty":  certainty,
			"timestamp":  timestamp.Format(time.RFC3339), // Convert timestamp to string
		}
		flaggedFlows = append(flaggedFlows, flowData)
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(flaggedFlows)
}

func get_home_dir() string {
	dirname, err := os.UserHomeDir()
	if err != nil {
		log.Fatal(err)
	}
	full_dir := dirname + utils.DB_PATH
	return full_dir
}
