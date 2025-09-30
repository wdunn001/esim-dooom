# C/C++ Development Quick Start for eSIM Doom

## Good News!

You **don't need Java** to develop for SIM cards! Here are your options:

## Option 1: Software SIM (Recommended Start)

Start with a pure C implementation that simulates a SIM card:

```bash
# In WSL2 Ubuntu
cd /mnt/c/Users/willi/esim-dooom
chmod +x setup_c_dev.sh
./setup_c_dev.sh
```

This creates a C-based framework without any Java dependencies.

## Option 2: MULTOS (For Real Hardware)

MULTOS cards support native C development:

**Pros:**
- Pure C, no Java
- Better performance than JavaCard
- More memory flexibility
- Real SIM cards available

**Cons:**
- Commercial SDK (but has eval versions)
- Less common than JavaCard

**Getting Started with MULTOS:**
1. Contact MULTOS International for evaluation SDK
2. Use their C compiler (based on standard C)
3. Deploy to real MULTOS cards

## Option 3: BasicCard

Supports C-like syntax in later versions:

**Pros:**
- No Java required
- Good documentation
- Hardware available
- Built-in file system

**Cons:**
- Proprietary language (ZC-Basic)
- Less flexible than pure C

## Your Development Path

### Phase 1: Software Development (No Java!)
```c
// Your main.c - Pure C, no Java
#include <stdint.h>

// SIM memory constraints
#define RAM_SIZE 8192  // 8KB

// Your Doom implementation
void doom_main() {
    // Pure C code here
}
```

### Phase 2: Choose Hardware Platform
1. **MULTOS** - Best for C developers
2. **BasicCard** - Good for prototyping
3. **Native ARM** - If you find compatible cards

### Phase 3: Port and Optimize
- Adapt memory usage
- Implement APDU interface
- Test on real hardware

## Quick Start Code Structure

```
esim-dooom/
├── src/
│   ├── sim/          # SIM card application (C)
│   │   ├── sim_main.c
│   │   └── apdu_handler.c
│   ├── doom/         # Doom8088 adaptation (C)
│   │   ├── doom_minimal.c
│   │   └── render_tiny.c
│   └── host/         # PC-side interface (C)
│       └── host_main.c
├── Makefile          # Standard C makefile
└── NO_JAVA_NEEDED    # :)
```

## Building Without Java

```bash
# Standard C compilation
make sim     # Build SIM application
make host    # Build host interface
make test    # Run tests

# No javac, no .class files, no JVM!
```

## Memory Budget (C Version)

```c
// Typical SIM card memory layout
// Much more efficient than Java!
typedef struct {
    uint8_t ram[8192];      // 8KB work RAM
    uint8_t eeprom[65536];  // 64KB persistent
} SimMemory;

// No Java overhead means more space for Doom!
```

## Next Steps

1. **Run the setup script** to create C development environment
2. **Look at src/sim/sim_main.c** - it's pure C, no Java
3. **Start porting Doom8088** - begin with rendering
4. **Test with software SIM** first
5. **Get MULTOS eval kit** for hardware testing

## Resources

- **MULTOS**: [www.multos.com](http://www.multos.com) - Request C SDK
- **swSIM**: Software SIM in C (tools/swsim)
- **PC/SC**: For card communication (already in C)

## Remember

- **No JavaCard needed** ✓
- **Pure C/C++ development** ✓
- **Multiple platform options** ✓
- **Start with software, move to hardware** ✓

You're free from Java! Happy C coding! 🎮
