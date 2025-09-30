# Deploying Text Doom to Real SIM Cards

## Overview

Text Doom is designed to run on actual SIM card hardware with just 8KB of memory.

## Supported Platforms

### 1. MULTOS Cards (Recommended)
- **Native C support** - no Java needed!
- Direct compilation and deployment
- Better performance than JavaCard

### 2. JavaCard
- Requires wrapping C code or porting to Java
- More common but less efficient
- Use GlobalPlatformPro for deployment

### 3. BasicCard
- Supports C-like syntax
- Good for prototyping

## APDU Interface

The game communicates via standard APDU commands:

| Command | CLA | INS | Description |
|---------|-----|-----|-------------|
| Init Game | 80 | 01 | Initialize game state |
| Send Input | 80 | 02 | Send keypress (WASD, etc) |
| Update Game | 80 | 03 | Process one game tick |
| Get Screen | 80 | 04 | Retrieve 40x25 text display |
| Get Status | 80 | 05 | Get health, ammo, level |
| Reset | 80 | 06 | Reset game |

## Deployment Steps

### For MULTOS:

```bash
# 1. Get MULTOS SDK from multos.com

# 2. Compile for MULTOS
mulgcc -O2 src/sim/sim_game_main.c -o text_doom.alu

# 3. Convert to loadable format  
multos-conv text_doom.alu text_doom.aif

# 4. Load onto card
multos-load -r reader0 text_doom.aif
```

### For JavaCard:

```bash
# 1. Install GlobalPlatformPro
wget https://github.com/martinpaljak/GlobalPlatformPro/releases/latest/download/gp.jar

# 2. You'll need to port the C code to Java or create a wrapper

# 3. Install on card
java -jar gp.jar --install text_doom.cap
```

## Hardware Requirements

- **Card Reader**: Any PC/SC compatible reader
- **Test Cards**: Blank MULTOS or JavaCard
- **Memory**: At least 8KB available RAM
- **PC Software**: 
  - PC/SC drivers (pcscd on Linux)
  - Platform SDK (MULTOS/JavaCard)

## Testing

1. Install `pcsc-tools`:
   ```bash
   sudo apt install pcscd pcsc-tools
   ```

2. Check card reader:
   ```bash
   pcsc_scan
   ```

3. Run host client:
   ```bash
   ./build/text_doom_host
   ```

## Memory Layout

The game uses approximately:
- Code: ~2KB
- Game state: ~3KB  
- Stack: ~1KB
- Heap: ~2KB
- **Total: <8KB**

## Performance

On typical SIM hardware (5-25 MHz):
- ~5-10 FPS expected
- 50-100ms input latency
- Playable but not fast

## Troubleshooting

1. **Card not detected**: Check reader drivers
2. **Out of memory**: Some cards report less than 8KB
3. **APDU errors**: Check command format
4. **Slow performance**: Normal for SIM cards!

## Why This Works

- Text mode = minimal memory
- No dynamic allocation
- Simple game logic
- Efficient data structures
- Pure computation (no I/O)
