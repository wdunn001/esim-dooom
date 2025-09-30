# eSIM Doom - Running Doom8088 on SIM Cards

## Overview

This project explores the fascinating capability of modern SIM cards and eSIMs to run complex applications by porting Doom8088 (a minimal Doom implementation for 8088 processors) to SIM card hardware. 

### Background

Modern SIM cards are actually small computers with:
- Dedicated processors (typically 32-bit ARM or similar)
- RAM (usually 64KB-512KB)
- Storage (several MB)
- Cryptographic co-processors
- Java Card or similar runtime environments

eSIMs are essentially virtual machines running these same environments, making them perfect candidates for running compact applications like Doom8088.

## Goals

1. **Primary Goal**: Successfully run Doom8088 on a SIM card
2. **Secondary Goals**:
   - Demonstrate the computational capabilities of modern SIM cards
   - Create a framework for running other retro games on SIM cards
   - Explore the limits of SIM card computing

## Technical Challenges

- **Memory Constraints**: SIM cards have very limited RAM compared to even 8088 systems
- **Execution Environment**: Java Card has significant restrictions
- **I/O Limitations**: No direct screen or keyboard access
- **Security Sandbox**: SIM cards run in highly restricted environments

## Architecture

```
┌─────────────────┐
│   Host Device   │
│  (Phone/Reader) │
└────────┬────────┘
         │ APDU Commands
┌────────┴────────┐
│  SIM Card / eSIM│
│  ┌────────────┐ │
│  │ Java Card  │ │
│  │   Runtime  │ │
│  └──────┬─────┘ │
│         │       │
│  ┌──────┴─────┐ │
│  │ Doom8088   │ │
│  │  Applet    │ │
│  └────────────┘ │
└─────────────────┘
```

## Development Setup

### Prerequisites

- Windows with WSL2 installed
- Ubuntu distro in WSL2
- C/C++ compiler (gcc/g++)
- SIM card reader (for testing)
- Doom8088 source code

### Development Options

**No Java Required!** This project supports multiple development approaches:

1. **Pure C Development** (Recommended)
   - Software SIM simulation in C
   - MULTOS platform for real cards
   - No JavaCard needed!

2. **JavaCard** (Traditional approach)
   - Standard but requires Java
   - See docs/DEVELOPMENT_TOOLS.md if you prefer this route

### Building

1. Open WSL2 Ubuntu:
   ```bash
   wsl -d Ubuntu
   ```

2. Quick setup for C development:
   ```bash
   cd /mnt/c/Users/willi/esim-dooom
   chmod +x setup_c_dev.sh
   ./setup_c_dev.sh  # Will prompt for sudo password to install packages
   ```

3. Build the C version (no Java needed!):
   ```bash
   make sim   # Build SIM application in pure C
   make host  # Build host interface
   make all   # Build everything
   ```

For detailed C/C++ development instructions, see `docs/C_DEVELOPMENT_QUICKSTART.md`

## Play the Game!

```bash
# Quick demo menu
./demo.sh

# Or run directly:
./build/play_text_doom    # Full game!
```

### Game Controls:
- **WASD** - Move
- **Q/E** - Turn left/right  
- **Space** - Shoot
- **R** - Restart when dead
- **ESC** - Quit

### Game Features:
- Complete gameplay loop
- Enemy AI
- Shooting mechanics
- Health/ammo pickups
- Win condition (find exit)
- All in pure ASCII text!

## Project Structure

```
esim-dooom/
├── .cursorules      # Development guidelines
├── .gitignore       # Git ignore rules
├── README.md        # This file
├── src/             # Source code
│   ├── javacard/    # Java Card applet code
│   ├── doom8088/    # Adapted Doom8088 code
│   └── host/        # Host-side communication
├── tools/           # Build and deployment tools
└── docs/            # Additional documentation
```

## Current Status

- [x] Project initialization
- [x] Environment setup
- [x] Doom8088 analysis for SIM adaptation
- [x] Memory optimization
- [x] Complete game implementation in C
- [x] Text-based rendering system
- [x] Playable game with enemies, shooting, and pickups
- [ ] Deployment to actual SIM card (requires hardware)

## Contributing

This is an experimental project exploring the limits of SIM card computing. Contributions, ideas, and feedback are welcome!

## License

This project builds upon Doom8088, which is based on the original Doom source code released under the GNU GPL license.

## Disclaimer

This is an experimental project for educational purposes. Modifying SIM cards may void warranties or violate carrier agreements. Always use test SIM cards and proper development environments.
