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

	"backend/executable"
	"backend/utils"
)

type Flow struct {
	ID                 int     `json:"id"`
	SourceIp           string  `json:"source_ip"`
	DestinationIp      string  `json:"destination_ip"`
	SourcePorts        string  `json:"source_ports"`
	DestinationPorts   string  `json:"destination_ports"`
	Protocol           int8    `json:"protocol"`
	TotalBytes         int     `json:"total_bytes"`
	Rate               float64 `json:"rate"`
	AveragePacketSize  float64 `json:"average_packet_size"`
	TotalPacketCount   int     `json:"total_packet_count"`
	StartTime          int     `json:"start_time"`
	LastUpdatedTime    int     `json:"last_updated_time"`
	Duration           int     `json:"duration"`
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

func checkIfExpired(w http.ResponseWriter) {
	row := DB.QueryRow("SELECT FIRST last_updated_time FROM flows")
	now := time.Now()
	var lastUpdatedTime int64
	if err := row.Scan(&lastUpdatedTime); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	lastUpdated := time.Unix(lastUpdatedTime, 0)
	
	if now.Sub(lastUpdated).Seconds() >= 7200 {
		executable.Expired = true;
		/* 
		 - Pause sniffer module
		 - Export data to external DB
		 - Ensure it was sent
		 - Delete db locally
		 - Create new db
		*/
	}
}

func createDB() {}

func deleteDB() {}

func FetchFlows(w http.ResponseWriter, r *http.Request) {
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

func get_home_dir() string {
	dirname, err := os.UserHomeDir()
	if err != nil {
		log.Fatal(err)
	}
	full_dir := dirname + utils.DB_PATH
	return full_dir
}

