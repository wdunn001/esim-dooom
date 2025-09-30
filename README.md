# Text Doom for SIM Cards

A complete, playable Doom-like game that runs on SIM cards with just 8KB of memory. Written in pure C with text-based graphics.

## Features

- **Complete gameplay**: Movement, shooting, enemies, health/ammo management
- **Text-only graphics**: Pure ASCII display (40x25 characters)
- **Tiny footprint**: Entire game fits in 8KB
- **SIM card ready**: APDU interface for real hardware deployment
- **No dependencies**: Pure C, no Java required

## Quick Start

```bash
# Build everything
make all

# Play the game
./build/play_text_doom
```

## Game Controls

- **WASD** - Move
- **Q/E** - Turn left/right
- **Space** - Shoot
- **R** - Restart (when dead)
- **ESC** - Quit

## What You'll See

```
########################################
#         E                            #
#    @>   *              a      +      #
#                               X      #
########################################
HP:085 AM:12 L:1
```

- `@` = You (player)
- `>` = Direction facing
- `E` = Enemy
- `*` = Bullet
- `#` = Wall
- `X` = Exit (goal)
- `a` = Ammo
- `+` = Health

## Project Structure

```
esim-dooom/
├── src/
│   ├── doom/
│   │   └── text_doom_game.c    # Core game logic
│   ├── sim/
│   │   ├── sim_game_main.c     # SIM card application
│   │   ├── apdu_handler.c      # APDU command processing
│   │   └── memory_manager.c    # Memory management
│   ├── host/
│   │   ├── host_game_client.c  # PC client for SIM
│   │   └── sim_interface.c     # SIM communication
│   └── test/
│       └── play_text_doom.c    # Standalone playable version
├── include/
│   └── sim_doom.h              # Common definitions
├── docs/
│   └── SIM_DEPLOYMENT.md       # How to deploy to real SIM cards
└── build/                      # Compiled executables
```

## Building

```bash
# Build SIM card application
make sim

# Build host client
make host  

# Build standalone game
make play

# Build everything
make all
```

## Testing the SIM Application

### Quick Test (No Hardware Needed)
```bash
# Build and run the APDU test harness
make test-sim
./build/test_sim_apdu
```

This simulates APDU commands to test the SIM application without hardware.

### Full Simulation
The project includes swSIM (software SIM simulator) in `tools/swsim/`. See [docs/TESTING_IN_SIMULATOR.md](docs/TESTING_IN_SIMULATOR.md) for setup instructions.

## Deploying to Real SIM Cards

This game can run on actual SIM card hardware. See [docs/SIM_DEPLOYMENT.md](docs/SIM_DEPLOYMENT.md) for:
- Supported platforms (MULTOS, JavaCard)
- APDU interface specification
- Deployment instructions
- Hardware requirements

## Technical Details

- **Memory usage**: ~3KB game state + ~2KB code
- **Language**: Pure C (C99)
- **Display**: 40x25 ASCII characters
- **Performance**: 5-10 FPS on SIM hardware

## Repository

Ready for GitHub:
```bash
git init
git add .
git commit -m "Text Doom - Complete game for SIM cards"
git remote add origin YOUR_GITHUB_URL
git push -u origin main
```

## License

This project is licensed under GPLv3 (same as Doom).

## Acknowledgments

- Inspired by the original Doom (id Software)
- Learned from Doom8088 text mode techniques
- Built specifically for SIM card constraints