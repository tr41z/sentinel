package executable

import (
	"bytes"
	"context"
	"fmt"
	"os"
	"os/exec"
	"sync"
	"time"

	"backend/utils"
)

// Sniffer health metrics
type SnifferHealth struct {
	Status     string    `json:"status"`
	Uptime     string    `json:"uptime"`
	ErrorCount int       `json:"error_count"`
	LastActive time.Time `json:"last_active"`
}

var (
	health       SnifferHealth
	healthMutex  sync.Mutex
	snifferStart time.Time
	Expired      bool
)

// GetSnifferHealth exposes the current health of the sniffer
func GetSnifferHealth() SnifferHealth {
	healthMutex.Lock()
	defer healthMutex.Unlock()

	if health.Status == "running" {
		health.Uptime = fmt.Sprintf("%.0f s", time.Since(snifferStart).Seconds())
	}

	return health
}

func updateHealthStatus(status string, lastActive time.Time, incrementError bool) {
	healthMutex.Lock()
	defer healthMutex.Unlock()

	health.Status = status
	if !lastActive.IsZero() {
		health.LastActive = lastActive
	}
	if incrementError {
		health.ErrorCount++
	}
}

func Invoke() {
	// Initialize sniffer health
	health = SnifferHealth{
		Status:     "initializing",
		ErrorCount: 0,
	}
	snifferStart = time.Now()

	// Check if the executable exists
	if _, err := os.Stat(utils.EXECUTABLE_PATH); os.IsNotExist(err) {
		updateHealthStatus("error", time.Time{}, true)
		fmt.Printf("Error: Sniffer executable not found at %s\n", utils.EXECUTABLE_PATH)
		os.Exit(1)
	}

	// Create a context with cancel function
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	cmd := exec.CommandContext(ctx, utils.EXECUTABLE_PATH)
	var stdout, stderr bytes.Buffer
	cmd.Stdout = &stdout
	cmd.Stderr = &stderr

	// Start the command
	fmt.Println("Running the sniffer executable...")
	updateHealthStatus("running", time.Now(), false)

	if err := cmd.Start(); err != nil {
		updateHealthStatus("error", time.Now(), true)
		fmt.Printf("Error starting sniffer: %s\n", err)
		os.Exit(1)
	}

	// Monitor for global condition
	go func() {
		for {
			if Expired {
				fmt.Println("Condition met, stopping the sniffer...")
				updateHealthStatus("stopping", time.Now(), false)
				cancel()
				return
			}
			time.Sleep(1 * time.Second)
		}
	}()

	// Wait for the command to finish
	if err := cmd.Wait(); err != nil {
		updateHealthStatus("error", time.Now(), true)
		fmt.Printf("Error running sniffer: %s\n", err)
		fmt.Printf("stderr: %s\n", stderr.String())
		os.Exit(1)
	}

	updateHealthStatus("stopped", time.Now(), false)
	fmt.Printf("Sniffer output:\n%s\n", stdout.String())
}
