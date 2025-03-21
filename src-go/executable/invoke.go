package executable

import (
	"context"
	"fmt"
	"os"
	"os/exec"
	"sync"
	"time"

	"backend/utils"
)

// Sniffer and AI module health metrics
type ModuleHealth struct {
	Status     string    `json:"status"`
	Uptime     string    `json:"uptime"`
	ErrorCount int       `json:"error_count"`
	LastActive time.Time `json:"last_active"`
}

var (
	// Sniffer
	healthSniffer       ModuleHealth
	healthSnifferMutex  sync.Mutex
	snifferStart        time.Time
	SnifferExpired      bool

	// AI Module
	healthAI       ModuleHealth
	healthAIMutex  sync.Mutex
	aiStart        time.Time
	AIExpired      bool
)

// GetSnifferHealth exposes the current health of the sniffer
func GetSnifferHealth() ModuleHealth {
	healthSnifferMutex.Lock()
	defer healthSnifferMutex.Unlock()

	if healthSniffer.Status == "running" {
		healthSniffer.Uptime = fmt.Sprintf("%.0f s", time.Since(snifferStart).Seconds())
	} else {
		healthSniffer.Uptime = "0"
	}

	return healthSniffer
}

// GetAIHealth exposes the current health of the AI module
func GetAIHealth() ModuleHealth {
	healthAIMutex.Lock()
	defer healthAIMutex.Unlock()

	if healthAI.Status == "running" {
		healthAI.Uptime = fmt.Sprintf("%.0f s", time.Since(aiStart).Seconds())
	} else {
		healthAI.Uptime = "0"
	}

	return healthAI
}

func updateSnifferHealth(status string, lastActive time.Time, incrementError bool) {
	healthSnifferMutex.Lock()
	defer healthSnifferMutex.Unlock()

	healthSniffer.Status = status
	if !lastActive.IsZero() {
		healthSniffer.LastActive = lastActive
	}
	if incrementError {
		healthSniffer.ErrorCount++
	}
}

func updateAIHealth(status string, lastActive time.Time, incrementError bool) {
	healthAIMutex.Lock()
	defer healthAIMutex.Unlock()

	healthAI.Status = status
	if !lastActive.IsZero() {
		healthAI.LastActive = lastActive
	}
	if incrementError {
		healthAI.ErrorCount++
	}
}

func InvokeSniffer() {
	runExecutable("sniffer", utils.SNIFFER_EXECUTABLE_PATH, &SnifferExpired, updateSnifferHealth, &snifferStart)
}

func InvokeAI() {
	runExecutable("AI module", utils.AI_EXECUTABLE_PATH, &AIExpired, updateAIHealth, &aiStart)
}

func runExecutable(name string, path string, expiredFlag *bool, updateHealth func(string, time.Time, bool), startTime *time.Time) {
	updateHealth("initializing", time.Time{}, false)
	*startTime = time.Now()

	if _, err := os.Stat(path); os.IsNotExist(err) {
		updateHealth("error", time.Time{}, true)
		fmt.Printf("Error: %s executable not found at %s\n", name, path)
		return
	}

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	cmd := exec.CommandContext(ctx, path)
	cmd.Stdout = nil
	cmd.Stderr = nil

	fmt.Printf("Running the %s executable...\n", name)
	updateHealth("running", time.Now(), false)

	if err := cmd.Start(); err != nil {
		updateHealth("error", time.Now(), true)
		fmt.Printf("Error starting %s: %s\n", name, err)
		return
	}

	go func() {
		for {
			if *expiredFlag {
				fmt.Printf("Stopping %s...\n", name)
				updateHealth("stopping", time.Now(), false)
				cancel()
				return
			}
			time.Sleep(1 * time.Second)
		}
	}()

	err := cmd.Wait()
	if ctx.Err() == context.Canceled {
		fmt.Printf("%s process terminated.\n", name)
	} else if err != nil {
		updateHealth("error", time.Now(), true)
		fmt.Printf("Error running %s: %s\n", name, err)
	}

	updateHealth("stopped", time.Now(), false)
}
