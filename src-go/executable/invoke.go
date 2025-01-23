package executable

import (
	"bytes"
	"context"
	"fmt"
	"os"
	"os/exec"
	"time"
)

// Global condition variable
var expired bool

func Invoke() {
	// Path to the sniffer executable
	snifferPath := "../sniffer-mod/build/sniffer.exe"

	// Check if the executable exists
	if _, err := os.Stat(snifferPath); os.IsNotExist(err) {
		fmt.Printf("Error: Sniffer executable not found at %s\n", snifferPath)
		os.Exit(1)
	}

	// Create a context with cancel function
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	// Run the executable with context
	cmd := exec.CommandContext(ctx, snifferPath)

	// Capture the output and error streams
	var stdout, stderr bytes.Buffer
	cmd.Stdout = &stdout
	cmd.Stderr = &stderr

	// Start the command
	fmt.Println("Running the sniffer executable...")
	if err := cmd.Start(); err != nil {
		fmt.Printf("Error starting sniffer: %s\n", err)
		os.Exit(1)
	}

	// Start a goroutine to switch expired to true after 5 seconds to imitate bool switch
	go func() {
		time.Sleep(150 * time.Second)
		expired = true
	}()

	// Check the global condition in a separate goroutine
	go func() {
		for {
			if expired {
				fmt.Println("Condition met, killing the sniffer process...")
				cancel()
				return
			}
			time.Sleep(1 * time.Second) // Check the condition every second
		}
	}()

	// Wait for the command to finish
	if err := cmd.Wait(); err != nil {
		fmt.Printf("Error running sniffer: %s\n", err)
		fmt.Printf("stderr: %s\n", stderr.String())
		os.Exit(1)
	}

	// Print the standard output
	fmt.Printf("Sniffer output:\n%s\n", stdout.String())
}
