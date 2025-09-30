#!/bin/bash
# Text Doom Launcher - Choose your version!

echo "╔══════════════════════════════════════════╗"
echo "║          TEXT DOOM LAUNCHER              ║"
echo "╚══════════════════════════════════════════╝"
echo ""
echo "Choose your version based on available memory:"
echo ""
echo "1) Minimal (8KB)  - Basic gameplay, works everywhere"
echo "2) Standard (32KB) - More enemies, larger maps"  
echo "3) Enhanced (64KB) - Maximum features"
echo "4) RAD-Doom (64KB) - Enhanced with COLOR & effects!"
echo "5) Memory Test - See what your card supports"
echo ""
echo -n "Select version (1-5): "
read choice

case $choice in
    1)
        echo "Building minimal version..."
        make minimal > /dev/null 2>&1
        ./build/play_text_doom
        ;;
    2)
        echo "Building standard version..."
        make standard > /dev/null 2>&1
        ./build/play_text_doom
        ;;
    3)
        echo "Building enhanced version..."
        make enhanced > /dev/null 2>&1
        ./build/play_text_doom
        ;;
    4)
        echo "Building RAD-Doom enhanced..."
        make rad-doom > /dev/null 2>&1
        ./build/play_rad_doom
        ;;
    5)
        echo "Running memory detection..."
        make memory-detect > /dev/null 2>&1
        ./build/memory_detect
        echo ""
        echo "Press Enter to return to menu..."
        read
        exec "$0"
        ;;
    *)
        echo "Invalid choice!"
        ;;
esac
