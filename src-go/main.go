package main

import (
	"backend/db"
	"backend/executable"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"sync"

	"github.com/rs/cors"
)

var (
	snifferControlMutex sync.Mutex
)

// HealthHandler exposes the health of the sniffer process
func HealthHandler(w http.ResponseWriter, r *http.Request) {
	health := executable.GetSnifferHealth()
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(health)
}

// StartSnifferHandler starts the sniffer process
func StartSnifferHandler(w http.ResponseWriter, r *http.Request) {
    snifferControlMutex.Lock()
    defer snifferControlMutex.Unlock()

    if executable.GetSnifferHealth().Status == "running" {
        http.Error(w, "Sniffer is already running", http.StatusBadRequest)
        return
    }

    // Reset the Expired flag to allow restarting
    executable.Expired = false

    go executable.Invoke() // Start the sniffer process
    w.WriteHeader(http.StatusOK)
    fmt.Fprintln(w, `{"status": "Sniffer started"}`)
}

// StopSnifferHandler stops the sniffer process
func StopSnifferHandler(w http.ResponseWriter, r *http.Request) {
	snifferControlMutex.Lock()
	defer snifferControlMutex.Unlock()

	if executable.GetSnifferHealth().Status != "running" {
		http.Error(w, "Sniffer is not running", http.StatusBadRequest)
		return
	}

	executable.Expired = true // Signal the sniffer process to stop
	w.WriteHeader(http.StatusOK)
	fmt.Fprintln(w, `{"status": "Sniffer stopping"}`)
}

func main() {
	db.InitDB()

	mux := http.NewServeMux()
	mux.HandleFunc("/api/v1/flows", db.FetchFlows)
	mux.HandleFunc("/api/v1/health", HealthHandler)
	mux.HandleFunc("/api/v1/sniffer/start", StartSnifferHandler)
	mux.HandleFunc("/api/v1/sniffer/stop", StopSnifferHandler)

	c := cors.New(cors.Options{
		AllowedOrigins:   []string{"http://localhost:5173", "http://localhost:5174"},
		AllowCredentials: true,
	})

	handler := c.Handler(mux)

	fmt.Println("Server is running at http://localhost:8080")
	log.Fatal(http.ListenAndServe(":8080", handler)) // Start the server on port 8080
}