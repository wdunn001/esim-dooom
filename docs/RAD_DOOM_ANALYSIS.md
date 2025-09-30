# RAD-Doom Analysis for eSIM Doom

## What RAD-Doom Actually Is

RAD-Doom is **not** Doom running on a C64. It's:
- Doom running on a **Raspberry Pi**
- The RPi **replaces the C64's CPU**
- The RPi uses the C64 as a display/sound device

### The Architecture:
```
┌─────────────────┐         ┌──────────────┐
│  Raspberry Pi   │────────►│   C64 VIC-II │
│ (Running Doom)  │         │  (Display)   │
│   Full Linux    │────────►│   C64 SID    │
│   32-bit ARM    │         │   (Sound)    │
└─────────────────┘         └──────────────┘
```

## What We CAN Learn from RAD-Doom

### 1. Color Reduction Techniques
- Reduces 320x200 RGB to 160x200 multicolor
- 4 colors per 4x8 block (C64 limitation)
- Smart dithering algorithms
- Background color heuristics

### 2. Display Techniques
```c
// RAD-Doom's approach:
// - Take RGB framebuffer
// - Downsample to 160x200
// - Apply dithering
// - Map to VIC-II palette
// - Enforce 4x8 block color restrictions
```

### 3. But Wait - C64 Has More Memory!
- C64: 64KB RAM
- SIM Card: 8KB RAM
- **Still 8x difference!**

## Can We Use RAD-Doom Techniques?

### What's Useful:
1. **Color reduction algorithm** - We need this for text mode
2. **Dithering patterns** - Can adapt for ASCII characters
3. **Block-based restrictions** - Similar to our character-based display
4. **50Hz stable rendering** - Good performance target

### What's NOT Useful:
1. **Requires Raspberry Pi** - We can't use external processors
2. **Full Doom engine** - Still way too big
3. **RGB framebuffer** - We don't have memory for this
4. **Floating point audio** - No FPU on SIM cards

## Adapting RAD-Doom Ideas for SIM Cards

### Color Mapping to ASCII:
```c
// RAD-Doom: Maps RGB to 16 C64 colors
// Our approach: Map brightness to ASCII characters

// Inspired by RAD-Doom's dithering
const char brightness_chars[] = " .:-=+*#%@";

// Instead of 4x8 color blocks, we have 1x1 character blocks
char map_brightness_to_ascii(uint8_t brightness) {
    return brightness_chars[brightness / 26];  // 0-255 to 0-9
}
```

### Simplified "Multicolor" Text Mode:
```c
// RAD-Doom: 4 colors per 4x8 block
// Our approach: 1 character per "pixel"

// Use different characters for different "colors"
#define WALL_FAR     '.'   // Like RAD-Doom's dark colors
#define WALL_MEDIUM  ':'   // Medium brightness
#define WALL_NEAR    '='   // Bright
#define WALL_CLOSE   '#'   // Very bright
```

## The Real Comparison:

| System | CPU | RAM | Display | Running Doom? |
|--------|-----|-----|---------|---------------|
| RAD-Doom | RPi 3 (1.4GHz ARM) | 1GB | C64 VIC-II | Full Doom |
| C64 Native | 6510 (1MHz) | 64KB | VIC-II | Too slow |
| eSIM Doom | SIM processor | 8KB | Text only | Ultra-minimal |

## What This Teaches Us:

1. **Even with clever tricks**, you need substantial hardware for "real" Doom
2. **Color reduction** and **dithering** are still useful concepts
3. **Block-based rendering** maps well to character-based display
4. We're on the right track with our text-mode approach

## Our Approach Remains:

```
RAD-Doom Concept          Our SIM Implementation
────────────────         ─────────────────────
RGB framebuffer     →    Direct text rendering
Dithering matrix    →    ASCII brightness levels
4x8 color blocks    →    1x1 character "pixels"
VIC-II output       →    APDU text transfer
50 FPS              →    Whatever SIM can do
```

## Try Our Version:

```bash
# Our 3D text-mode demo (8KB, no external CPU!)
./build/visual_doom8088

# Implements similar concepts but MUCH simpler
```

RAD-Doom shows that even with a Raspberry Pi helping, rendering Doom is complex. Our SIM card version has to be **radically** simplified!
