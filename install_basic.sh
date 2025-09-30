#!/bin/bash
# Basic installation script - installs only essential tools

echo "Installing basic C development tools for eSIM Doom..."
echo "This will only install essential packages."
echo ""

# Check if running in WSL
if [ ! -f /proc/sys/fs/binfmt_misc/WSLInterop ]; then
    echo "Warning: This doesn't appear to be WSL. Continue anyway? (y/n)"
    read -r response
    if [ "$response" != "y" ]; then
        exit 1
    fi
fi

# Install only the essentials
echo "Installing essential packages..."
sudo apt update
sudo apt install -y build-essential git make

echo ""
echo "Basic installation complete!"
echo ""
echo "Optional packages (install later if needed):"
echo "  - libpcsclite-dev pcscd pcsc-tools  (for real SIM card access)"
echo "  - gcc-multilib g++-multilib         (for 32-bit builds)"
echo "  - valgrind gdb                      (for debugging)"
echo ""
echo "To install optional packages later:"
echo "  sudo apt install libpcsclite-dev pcscd pcsc-tools"
echo ""
echo "You can now build the software SIM version with:"
echo "  make sim"
