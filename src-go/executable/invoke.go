package executable;

import (
	"bytes"
	"fmt"
	"os"
	"os/exec"
)

func Invoke() {
	// Path to the sniffer executable
	snifferPath := "../sniffer-mod/build/sniffer.exe"

	// Check if the executable exists
	if _, err := os.Stat(snifferPath); os.IsNotExist(err) {
		fmt.Printf("Error: Sniffer executable not found at %s\n", snifferPath)
		os.Exit(1)
	}

	// Run the executable
	cmd := exec.Command(snifferPath)

	// Capture the output and error streams
	var stdout, stderr bytes.Buffer
	cmd.Stdout = &stdout
	cmd.Stderr = &stderr

	// Start the command
	fmt.Println("Running the sniffer executable...")
	if err := cmd.Run(); err != nil {
		fmt.Printf("Error running sniffer: %s\n", err)
		fmt.Printf("stderr: %s\n", stderr.String())
		os.Exit(1)
	}

	// Print the standard output
	fmt.Printf("Sniffer output:\n%s\n", stdout.String())
}
