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

# New game SIM version
GAME_SIM_SOURCES = src/sim/sim_game_main.c

HOST_SOURCES = src/host/host_main.c \
               src/host/sim_interface.c

# New game host client
GAME_HOST_SOURCES = src/host/host_game_client.c

# Build software SIM version
sim: $(SIM_SOURCES)
	$(CC) $(CFLAGS) -o build/esim_doom_sim $(SIM_SOURCES)

# Build game SIM version
game_sim: $(GAME_SIM_SOURCES)
	$(CC) $(CFLAGS) -o build/game_doom_sim $(GAME_SIM_SOURCES)

# Build host interface
host: $(HOST_SOURCES)
	$(CC) $(CFLAGS) -o build/esim_doom_host $(HOST_SOURCES)
	# Note: Add -lpcsclite when PC/SC is installed

# Build game host client
game_host: $(GAME_HOST_SOURCES)
	$(CC) $(CFLAGS) -o build/game_doom_host $(GAME_HOST_SOURCES)

# Visual test
visual: src/test/visual_test.c
	$(CC) $(CFLAGS) -o build/visual_test src/test/visual_test.c

# Doom8088-style visual test  
visual3d: src/test/visual_doom8088_style.c
	$(CC) $(CFLAGS) -o build/visual_doom8088 src/test/visual_doom8088_style.c -lm

# Playable text Doom game
play: src/test/play_text_doom.c
	$(CC) $(CFLAGS) -o build/play_text_doom src/test/play_text_doom.c

# Build all
all: sim host visual play game_sim game_host

clean:
	rm -rf build/*

.PHONY: all sim host visual clean
