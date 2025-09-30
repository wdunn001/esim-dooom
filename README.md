# Text Doom for SIM Cards

A complete, playable Doom-like game that runs on SIM cards. Designed for 8KB RAM minimum, scales to 64KB+ for enhanced features. Written in pure C with text-based graphics.

## Features

- **Complete gameplay**: Movement, shooting, enemies, health/ammo management
- **Text-only graphics**: Pure ASCII display (40x25 characters)
- **Scalable memory**: Works on 8KB cards, scales up to 64KB+
- **SIM card ready**: APDU interface for real hardware deployment
- **No dependencies**: Pure C

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

### Default Build (8KB)
```bash
# Build everything for minimal 8KB cards
make all
```

### Memory-Specific Builds
```bash
# Show available memory configurations
make memory-info

# Build for 8KB cards (basic features)
make minimal

# Build for 32KB cards (save states, more enemies)
make standard

# Build for 64KB+ cards (large maps, particle effects)
make enhanced

# Test memory detection
make memory-detect && ./build/memory_detect

# Build RAD-Doom Enhanced (64KB cards with color!)
make rad-doom && ./build/play_rad_doom
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

## Why 8KB Default?

SIM cards have different memory types:
- **EEPROM/Flash**: 32KB-512KB (persistent storage)
- **RAM**: 2KB-64KB (working memory - this is our limit!)

We target 8KB RAM to ensure compatibility with basic SIM cards. Modern eSIMs often have 32-64KB RAM, allowing enhanced features. See [docs/MEMORY_CONFIGURATIONS.md](docs/MEMORY_CONFIGURATIONS.md) for details.

## RAD-Doom Enhanced Edition

For modern eSIMs with 64KB+ RAM, try the **RAD-Doom Enhanced** version with:
- 🌈 Full ANSI color support
- 🎨 Dithering effects inspired by C64 RAD-Doom
- 💥 Particle effects for explosions
- 🗺️ Massive 60x40 maps (3x larger!)
- 🤖 Up to 20 enemies with advanced AI

See [docs/RAD_DOOM_ENHANCED.md](docs/RAD_DOOM_ENHANCED.md) for details.

## Technical Details

- **Memory usage**: ~2KB game state (8KB config), scales with RAM
- **Language**: Pure C (C99)
- **Display**: 40x25 ASCII (up to 80x30 on 64KB cards)
- **Performance**: 5-10 FPS on SIM hardware

## License

This project is licensed under GPLv3 (same as Doom).

## Acknowledgments

- Inspired by the original Doom (id Software)
- Built specifically for SIM card constraints