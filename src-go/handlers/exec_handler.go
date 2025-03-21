package handlers

import (
	"backend/executable"
	"encoding/json"
	"fmt"
	"net/http"
	"sync"
)

var (
	snifferControlMutex sync.Mutex
	aiControlMutex      sync.Mutex
)

// HealthHandler exposes the health of the sniffer process
func HealthHandler(w http.ResponseWriter, r *http.Request) {
	health := executable.GetSnifferHealth()
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(health)
}

// AI Health Handler
func AIHealthHandler(w http.ResponseWriter, r *http.Request) {
	health := executable.GetAIHealth()
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(health)
}

// Start Sniffer
func StartSnifferHandler(w http.ResponseWriter, r *http.Request) {
	snifferControlMutex.Lock()
	defer snifferControlMutex.Unlock()

	if executable.GetSnifferHealth().Status == "running" {
		http.Error(w, "Sniffer is already running", http.StatusBadRequest)
		return
	}

	executable.SnifferExpired = false
	go executable.InvokeSniffer()
	w.WriteHeader(http.StatusOK)
	fmt.Fprintln(w, `{"status": "Sniffer started"}`)
}

// Stop Sniffer
func StopSnifferHandler(w http.ResponseWriter, r *http.Request) {
	snifferControlMutex.Lock()
	defer snifferControlMutex.Unlock()

	if executable.GetSnifferHealth().Status != "running" {
		http.Error(w, "Sniffer is not running", http.StatusBadRequest)
		return
	}

	executable.SnifferExpired = true
	w.WriteHeader(http.StatusOK)
	fmt.Fprintln(w, `{"status": "Sniffer stopping"}`)
}

// Start AI Module
func StartAIHandler(w http.ResponseWriter, r *http.Request) {
	aiControlMutex.Lock()
	defer aiControlMutex.Unlock()

	if executable.GetAIHealth().Status == "running" {
		http.Error(w, "AI module is already running", http.StatusBadRequest)
		return
	}

	executable.AIExpired = false
	go executable.InvokeAI()
	w.WriteHeader(http.StatusOK)
	fmt.Fprintln(w, `{"status": "AI module started"}`)
}

// Stop AI Module
func StopAIHandler(w http.ResponseWriter, r *http.Request) {
	aiControlMutex.Lock()
	defer aiControlMutex.Unlock()

	if executable.GetAIHealth().Status != "running" {
		http.Error(w, "AI module is not running", http.StatusBadRequest)
		return
	}

	executable.AIExpired = true
	w.WriteHeader(http.StatusOK)
	fmt.Fprintln(w, `{"status": "AI module stopping"}`)
}
