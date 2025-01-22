package main

import (
	"backend/db"
	"backend/executable"
	"net/http"
	"fmt"
	"log"
)

func main() {
	db.InitDB()
	http.HandleFunc("/api/v1/flows", db.FetchFlows)

	fmt.Println("Server is running at http://localhost:8080")
 	log.Fatal(http.ListenAndServe(":8080", nil)) // Start the server on port 8080
	executable.Invoke();
}