# eSIM Doom Makefile - Text-based Doom for SIM Cards
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -Iinclude

# Memory configuration (default: MINIMAL for 8KB)
# Options: MINIMAL (8KB), STANDARD (32KB), ENHANCED (64KB+)
MEMORY ?= MINIMAL

# Core game files
GAME_SOURCES = src/doom/text_doom_game.c

# SIM card application
GAME_SIM_SOURCES = src/sim/sim_game_main.c

# Host client for SIM communication
GAME_HOST_SOURCES = src/host/host_game_client.c

# Supporting SIM infrastructure
SIM_SUPPORT = src/sim/apdu_handler.c \
              src/sim/memory_manager.c

# Host infrastructure
HOST_SUPPORT = src/host/sim_interface.c

# Build SIM card application (with test main for standalone testing)
sim: $(GAME_SIM_SOURCES)
	$(CC) $(CFLAGS) -DTEST_BUILD -o build/text_doom_sim $(GAME_SIM_SOURCES)

# Build host client
host: $(GAME_HOST_SOURCES)
	$(CC) $(CFLAGS) -o build/text_doom_host $(GAME_HOST_SOURCES)

# Build playable standalone version
play: src/test/play_text_doom.c
	$(CC) $(CFLAGS) -o build/play_text_doom src/test/play_text_doom.c

# Build SIM APDU test harness
test-sim: src/test/test_sim_apdu.c
	$(CC) $(CFLAGS) -o build/test_sim_apdu src/test/test_sim_apdu.c

# Build memory detection demo
memory-detect: src/test/memory_detect.c
	$(CC) $(CFLAGS) -o build/memory_detect src/test/memory_detect.c

# Build RAD-Doom enhanced version (32KB+ cards)
rad-doom: src/test/play_rad_doom.c
	$(CC) $(CFLAGS) -DUSE_CONFIG_HEADER -DMEMORY_CONFIG=3 -o build/play_rad_doom src/test/play_rad_doom.c

# Build all
all: sim host play test-sim

# Clean
clean:
	rm -rf build/*

# Build for specific memory configurations
minimal:
	$(MAKE) MEMORY=MINIMAL all

standard:
	$(MAKE) MEMORY=STANDARD CFLAGS="$(CFLAGS) -DUSE_CONFIG_HEADER -DMEMORY_CONFIG=2" all

enhanced:
	$(MAKE) MEMORY=ENHANCED CFLAGS="$(CFLAGS) -DUSE_CONFIG_HEADER -DMEMORY_CONFIG=3" all

# Show memory usage
memory-info:
	@echo "=== Text Doom Memory Configurations ==="
	@echo "minimal:  8KB RAM - Basic gameplay (3 enemies, small map)"
	@echo "standard: 32KB RAM - Enhanced features (10 enemies, save states)"
	@echo "enhanced: 64KB+ RAM - Full features (20 enemies, large maps)"
	@echo ""
	@echo "Build with: make [minimal|standard|enhanced]"

# Install on SIM card (requires PC/SC tools)
install-sim: sim
	@echo "To install on a real SIM card:"
	@echo "1. Use GlobalPlatformPro for JavaCard"
	@echo "2. Use MULTOS tools for MULTOS cards"
	@echo "3. See docs/SIM_DEPLOYMENT.md for details"

.PHONY: all sim host play clean install-sim minimal standard enhanced memory-info