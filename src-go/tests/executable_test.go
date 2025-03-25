package tests

import (
	"backend/executable"
	"testing"
	"time"
)

func TestGetSnifferHealth(t *testing.T) {
	executable.RunExecutableFunc = func(name string, path string, expiredFlag *bool, updateHealth func(string, time.Time, bool), startTime *time.Time) {
		updateHealth("running", time.Now(), false)
	}

	executable.SnifferExpired = false
	executable.InvokeSniffer()
	time.Sleep(1 * time.Second) // Allow some time for the sniffer to initialize

	health := executable.GetSnifferHealth()
	if health.Status != "running" {
		t.Errorf("Expected status 'running', got '%s'", health.Status)
	}
}

func TestGetAIHealth(t *testing.T) {
	executable.RunExecutableFunc = func(name string, path string, expiredFlag *bool, updateHealth func(string, time.Time, bool), startTime *time.Time) {
		updateHealth("running", time.Now(), false)
	}

	executable.AIExpired = false
	executable.InvokeAI()
	time.Sleep(1 * time.Second) // Allow some time for the AI module to initialize

	health := executable.GetAIHealth()
	if health.Status != "running" {
		t.Errorf("Expected status 'running', got '%s'", health.Status)
	}
}
