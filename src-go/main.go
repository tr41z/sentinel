package main

import (
	"backend/db"
	"backend/executable"
	"net/http"
	"fmt"
	"log"

	"github.com/rs/cors"
)

func main() {
    db.InitDB()
    
    mux := http.NewServeMux()
    mux.HandleFunc("/api/v1/flows", db.FetchFlows)

    go executable.Invoke() // async invoke of sniffer process

    c := cors.New(cors.Options{
        AllowedOrigins: []string{"http://localhost:5173", "http://localhost:5174"},
        AllowCredentials: true,
    })

    handler := c.Handler(mux)

    fmt.Println("Server is running at http://localhost:8080")
    log.Fatal(http.ListenAndServe(":8080", handler)) // Start the server on port 8080
}