#!/bin/bash
# Run Text Doom

echo "=== TEXT DOOM ==="
echo "A complete Doom-like game in 8KB!"
echo ""
echo "Building game..."
make play

if [ $? -eq 0 ]; then
    echo "Starting game..."
    echo ""
    ./build/play_text_doom
else
    echo "Build failed! Run 'make play' to see errors."
fi
