#!/bin/bash
# Quick demo script to show Text Doom

echo "=== eSIM DOOM DEMO ==="
echo "Text-based Doom that fits in 8KB!"
echo ""
echo "We have several demos to show:"
echo "1. Full playable game"
echo "2. 3D ASCII rendering" 
echo "3. Basic movement test"
echo ""
echo "Which would you like to try? (1-3, or 'q' to quit): "
read choice

case $choice in
    1)
        echo "Starting full Text Doom game..."
        echo "Controls: WASD=move, QE=turn, SPACE=shoot"
        sleep 2
        ./build/play_text_doom
        ;;
    2)
        echo "Starting 3D ASCII renderer..."
        echo "This shows Doom8088-style rendering in text"
        sleep 2
        ./build/visual_doom8088
        ;;
    3)
        echo "Starting basic movement demo..."
        echo "Simple 2D view with collision detection"
        sleep 2
        ./build/visual_test
        ;;
    q|Q)
        echo "Thanks for checking out eSIM Doom!"
        ;;
    *)
        echo "Invalid choice. Run ./demo.sh again"
        ;;
esac

echo ""
echo "This entire game fits in SIM card memory (8KB)!"
echo "Written in pure C with no Java dependencies."
