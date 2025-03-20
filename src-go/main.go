package main

import (
	"backend/db"
	"backend/handlers"
	"fmt"
	"log"
	"net/http"

	"github.com/rs/cors"
)

func main() {
	db.InitDB()

	mux := http.NewServeMux()
	mux.HandleFunc("/api/v1/flows", db.FetchFlows)
	mux.HandleFunc("/api/v1/threats", db.FetchThreats)
	mux.HandleFunc("/api/v1/flagged_ips", db.FetchFlaggedIPs)

	// Sniffer Module Endpoints
	mux.HandleFunc("/api/v1/health", handlers.HealthHandler)
	mux.HandleFunc("/api/v1/sniffer/start", handlers.StartSnifferHandler)
	mux.HandleFunc("/api/v1/sniffer/stop", handlers.StopSnifferHandler)

	// AI Module Endpoints
	mux.HandleFunc("/api/v1/ai/health", handlers.AIHealthHandler)
	mux.HandleFunc("/api/v1/ai/start", handlers.StartAIHandler)
	mux.HandleFunc("/api/v1/ai/stop", handlers.StopAIHandler)

	c := cors.New(cors.Options{
		AllowedOrigins:   []string{"http://localhost:*"},
		AllowCredentials: true,
	})

	handler := c.Handler(mux)

	fmt.Println("Server is running at http://localhost:8080")
	log.Fatal(http.ListenAndServe(":8080", handler))
}
