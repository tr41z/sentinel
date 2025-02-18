package main

import (
	"backend/db"
	"fmt"
	"log"
	"net/http"
	"github.com/rs/cors"
	"backend/handlers"
)


func main() {
	db.InitDB()

	mux := http.NewServeMux()
	mux.HandleFunc("/api/v1/flows", db.FetchFlows)
	mux.HandleFunc("/api/v1/health", handlers.HealthHandler)
	mux.HandleFunc("/api/v1/sniffer/start", handlers.StartSnifferHandler)
	mux.HandleFunc("/api/v1/sniffer/stop", handlers.StopSnifferHandler)

	c := cors.New(cors.Options{
		AllowedOrigins:   []string{"http://localhost:*"},
		AllowCredentials: true,
	})

	handler := c.Handler(mux)

	fmt.Println("Server is running at http://localhost:8080")
	log.Fatal(http.ListenAndServe(":8080", handler)) // Start the server on port 8080
}
