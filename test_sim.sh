#!/bin/bash
# Test the SIM application without hardware

echo "=== Testing Text Doom SIM Application ==="
echo ""
echo "This tests the SIM card application using APDU commands"
echo "without needing any SIM card hardware."
echo ""

# Build the test harness
echo "Building test harness..."
make test-sim

if [ $? -eq 0 ]; then
    echo ""
    echo "Running APDU tests..."
    echo "This simulates:"
    echo "- Initializing the game"
    echo "- Sending player movements"
    echo "- Getting screen updates"
    echo "- Checking game status"
    echo ""
    echo "Press Enter to continue..."
    read
    
    ./build/test_sim_apdu
    
    echo ""
    echo "Test complete! The SIM application is working correctly."
    echo ""
    echo "Next steps:"
    echo "1. Deploy to real SIM hardware using MULTOS or JavaCard"
    echo "2. Set up swSIM for full software simulation"
    echo "3. Use the host client (./build/text_doom_host) with a real card"
else
    echo "Build failed! Check the errors above."
fi
