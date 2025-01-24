package main

import (
	"backend/db"
	"backend/executable"
	"encoding/json"
	"fmt"
	"log"
	"net/http"

	"github.com/rs/cors"
)

// HealthHandler exposes the health of the sniffer process
func HealthHandler(w http.ResponseWriter, r *http.Request) {
	health := executable.GetSnifferHealth()
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(health)
}

func main() {
	go executable.Invoke() // async invoke of sniffer process

	db.InitDB()

	mux := http.NewServeMux()
	mux.HandleFunc("/api/v1/flows", db.FetchFlows)
	mux.HandleFunc("/api/v1/health", HealthHandler) // New endpoint for health check

	c := cors.New(cors.Options{
		AllowedOrigins:   []string{"http://localhost:5173", "http://localhost:5174"},
		AllowCredentials: true,
	})

	handler := c.Handler(mux)

	fmt.Println("Server is running at http://localhost:8080")
	log.Fatal(http.ListenAndServe(":8080", handler)) // Start the server on port 8080
}
