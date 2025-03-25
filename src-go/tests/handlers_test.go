package tests

import (
	"backend/executable"
	"backend/handlers"
	"net/http"
	"net/http/httptest"
	"testing"
)

func TestHealthHandler(t *testing.T) {
	executable.GetSnifferHealthFunc = func() executable.ModuleHealth {
		return executable.ModuleHealth{Status: "running"}
	}
	req := httptest.NewRequest("GET", "/api/v1/sniffer/health", nil)
	w := httptest.NewRecorder()

	handlers.HealthHandler(w, req)

	if w.Code != http.StatusOK {
		t.Errorf("Expected status 200, got %d", w.Code)
	}
}

func TestStartSnifferHandler(t *testing.T) {
	executable.GetSnifferHealthFunc = func() executable.ModuleHealth {
		return executable.ModuleHealth{Status: "stopped"}
	}
	req := httptest.NewRequest("POST", "/api/v1/sniffer/start", nil)
	w := httptest.NewRecorder()

	handlers.StartSnifferHandler(w, req)

	if w.Code != http.StatusOK {
		t.Errorf("Expected status 200, got %d", w.Code)
	}
}

func TestStopSnifferHandler(t *testing.T) {
	executable.GetSnifferHealthFunc = func() executable.ModuleHealth {
		return executable.ModuleHealth{Status: "running"}
	}
	req := httptest.NewRequest("POST", "/api/v1/sniffer/stop", nil)
	w := httptest.NewRecorder()

	handlers.StopSnifferHandler(w, req)

	if w.Code != http.StatusOK {
		t.Errorf("Expected status 200, got %d", w.Code)
	}
}
