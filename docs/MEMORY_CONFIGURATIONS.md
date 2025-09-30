# SIM Card Memory Configurations

## Understanding SIM Card Memory

SIM cards have three types of memory:

1. **ROM** (Read-Only Memory) - Contains the card OS
2. **EEPROM/Flash** (32KB-512KB+) - Persistent storage for contacts, SMS, files
3. **RAM** (2KB-64KB) - Working memory for running applications

## Why We Target 8KB

The 8KB limit is for **RAM** (working memory), not total storage:

- **Basic SIM cards**: 2-8KB RAM
- **Java Card Classic**: 8-16KB RAM  
- **Modern USIM/eSIM**: 32-64KB RAM
- **High-end eSIM**: 128KB+ RAM

We target 8KB to ensure compatibility with the widest range of cards.

## Memory Configurations

### Minimal (8KB) - Current Default
```c
// In text_doom_game.c
#define MAX_ENEMIES 3
#define MAX_BULLETS 5
#define MAX_PICKUPS 2
#define MAP_W 20
#define MAP_H 20
```

### Standard (32KB)
```c
#define MAX_ENEMIES 10
#define MAX_BULLETS 20
#define MAX_PICKUPS 8
#define MAP_W 40
#define MAP_H 30
#define ENABLE_MULTIPLE_LEVELS 1
#define ENABLE_SAVE_STATES 1
```

### Enhanced (64KB+)
```c
#define MAX_ENEMIES 20
#define MAX_BULLETS 50
#define MAX_PICKUPS 15
#define MAP_W 60
#define MAP_H 40
#define ENABLE_MULTIPLE_LEVELS 1
#define ENABLE_SAVE_STATES 1
#define ENABLE_SOUND_EFFECTS 1
#define ENABLE_PARTICLE_EFFECTS 1
```

## Detecting Available Memory

For MULTOS cards:
```c
uint16_t get_available_ram() {
    // MULTOS API call
    return multos_query_memory();
}
```

For JavaCard:
```java
short availableMemory = JCSystem.getAvailableMemory(
    JCSystem.MEMORY_TYPE_TRANSIENT_RESET);
```

## Building for Different Targets

```bash
# Minimal (8KB)
make CFLAGS="-DMEMORY_CONFIG=MINIMAL"

# Standard (32KB)
make CFLAGS="-DMEMORY_CONFIG=STANDARD"

# Enhanced (64KB)
make CFLAGS="-DMEMORY_CONFIG=ENHANCED"
```

## Features by Memory Size

| Feature | 8KB | 32KB | 64KB |
|---------|-----|------|------|
| Basic gameplay | ✓ | ✓ | ✓ |
| Multiple weapons | - | ✓ | ✓ |
| Save states | - | ✓ | ✓ |
| Multiple levels | - | ✓ | ✓ |
| Larger maps | - | ✓ | ✓ |
| More enemies | - | ✓ | ✓ |
| Particle effects | - | - | ✓ |
| Sound patterns | - | - | ✓ |

## eSIM Advantages

Modern eSIMs often provide:
- **More RAM**: 32-256KB typical
- **Faster processors**: 32-bit ARM cores
- **Better APIs**: Direct memory access
- **Native C support**: No Java overhead

## Optimizing for Your Card

1. Check your card's specifications
2. Adjust memory constants
3. Enable additional features
4. Test with the simulator

## Example: 64KB Configuration

Create `src/doom/text_doom_enhanced.c`:
```c
// Enhanced version for 64KB cards
#define MEMORY_CONFIG ENHANCED
#include "text_doom_game.c"

// Additional features
typedef struct {
    uint8_t type;
    uint8_t damage;
    uint8_t ammo_use;
} Weapon;

Weapon weapons[] = {
    {0, 10, 1},  // Pistol
    {1, 30, 1},  // Shotgun
    {2, 50, 5},  // Rocket launcher
};
```

## Memory Usage Breakdown

### Current (8KB):
- Game state: ~2.1KB
- Screen buffer: 1KB  
- Stack: ~1KB
- Code/constants: ~3KB
- Free: ~1KB

### With 64KB:
- Game state: ~10KB
- Multiple screen buffers: 4KB
- Larger maps: 8KB
- Save states: 4KB
- Enhanced features: 10KB
- Still plenty free!

## Recommendations

- **For testing**: Use 8KB version (maximum compatibility)
- **For modern eSIM**: Use 32KB or 64KB versions
- **For specific deployment**: Match your target hardware

The game scales automatically based on available memory!
