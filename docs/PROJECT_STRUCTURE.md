# Text Doom Project Structure

## Clean, Minimal Project Ready for GitHub and SIM Deployment

### Core Files

#### Game Logic
- `src/doom/text_doom_game.c` - Complete game implementation (~500 lines)
  - Player movement and collision
  - Enemy AI
  - Shooting mechanics  
  - Pickups and level exit
  - Text-based rendering

#### SIM Card Application
- `src/sim/sim_game_main.c` - SIM card APDU interface
  - Processes game commands via APDU
  - Manages game state in SIM memory
  - Returns screen data to host

- `src/sim/apdu_handler.c` - APDU command processing
- `src/sim/memory_manager.c` - Memory management for SIM

#### Host Interface
- `src/host/host_game_client.c` - PC client to communicate with SIM
  - Sends APDU commands
  - Displays game screen
  - Handles user input

- `src/host/sim_interface.c` - SIM card communication layer

#### Standalone Test
- `src/test/play_text_doom.c` - Playable version for testing
  - Runs the complete game on PC
  - No SIM card needed

### Build System
- `Makefile` - Clean, simple build configuration
  - `make sim` - Build SIM application
  - `make host` - Build host client
  - `make play` - Build standalone game
  - `make all` - Build everything

### Documentation
- `README.md` - Project overview and quick start
- `docs/SIM_DEPLOYMENT.md` - How to deploy to real SIM cards
- `.cursorules` - Development guidelines

### Git Configuration
- `.gitignore` - Excludes build artifacts
- `.gitattributes` - Line ending normalization

### Scripts
- `run_game.sh` - Quick launcher for the game

## Executables (after building)

- `build/text_doom_sim` - SIM card application (21KB)
- `build/text_doom_host` - Host client (17KB)  
- `build/play_text_doom` - Standalone game (21KB)

## Memory Usage

Total game state: ~2.1KB
- Fits easily in 8KB SIM card memory
- No dynamic allocation
- Pure C implementation

## Ready for:

1. **GitHub** - Clean structure, proper documentation
2. **SIM Deployment** - APDU interface implemented
3. **Testing** - Standalone version works immediately
