#!/bin/bash
# Setup script for C-based SIM card development environment

echo "Setting up C-based SIM card development environment..."

# Create directory structure
mkdir -p src/{sim,doom,host,common}
mkdir -p include
mkdir -p tools
mkdir -p test
mkdir -p build

# Install required packages
echo "Installing required packages..."
sudo apt update
sudo apt install -y \
    build-essential \
    gcc-multilib \
    g++-multilib \
    libpcsclite-dev \
    pcscd \
    pcsc-tools \
    git \
    make \
    cmake \
    valgrind \
    gdb

# Clone swSIM for reference and testing
if [ ! -d "tools/swsim" ]; then
    echo "Cloning swSIM (software SIM implementation)..."
    cd tools
    git clone https://github.com/tomasz-lisowski/swsim
    cd swsim
    # Try to build it (may need adjustments)
    make || echo "swSIM build failed - manual intervention may be needed"
    cd ../..
fi

# Create initial Makefile
cat > Makefile << 'EOF'
# eSIM Doom Makefile
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -O2 -std=c99 -Iinclude
CXXFLAGS = -Wall -Wextra -O2 -std=c++11 -Iinclude

# Target for software SIM version
SIM_SOURCES = src/sim/sim_main.c \
              src/sim/apdu_handler.c \
              src/sim/memory_manager.c \
              src/doom/doom_minimal.c

HOST_SOURCES = src/host/host_main.c \
               src/host/sim_interface.c

# Build software SIM version
sim: $(SIM_SOURCES)
	$(CC) $(CFLAGS) -o build/esim_doom_sim $(SIM_SOURCES)

# Build host interface
host: $(HOST_SOURCES)
	$(CC) $(CFLAGS) -o build/esim_doom_host $(HOST_SOURCES) -lpcsclite

# Build all
all: sim host

clean:
	rm -rf build/*

.PHONY: all sim host clean
EOF

echo "Development environment setup complete!"
echo ""
echo "Next steps:"
echo "1. Run 'make' to build the initial framework"
echo "2. Check tools/swsim for software SIM reference"
echo "3. Start implementing minimal Doom in src/doom/"
echo "4. See docs/C_CPP_ALTERNATIVES.md for development options"
