# What We Can Leverage from RAD-Doom

## The Reality Check

RAD-Doom is **amazing** but it's not actually running on C64 hardware:
- It runs on a **Raspberry Pi** (1GB RAM, 1.4GHz CPU)
- The Pi just uses the C64 as a display
- Still way beyond SIM card capabilities

## But We CAN Steal These Ideas!

### 1. Color/Brightness Reduction Algorithm
**RAD-Doom**: Reduces RGB → 16 C64 colors with smart dithering
**Our Version**: Reduce brightness → 10 ASCII characters
```c
// RAD-Doom: Complex color mapping
// Our version:
const char ascii_palette[] = " .:-=+*#%@";
```

### 2. Dithering Matrices
**RAD-Doom**: Bayer matrices, blue noise, temporal dithering
**Our Version**: Simple 2x2 Bayer for text
```c
// Creates smoother gradients in text mode
static const uint8_t bayer_2x2[2][2] = {{0,2},{3,1}};
```

### 3. Block Constraints
**RAD-Doom**: 4 colors per 4x8 block (VIC-II limit)
**Our Version**: Brightness coherence per 2x2 text block
```c
// Ensures readability by avoiding too much contrast
// in small areas
```

### 4. Frame Alternation
**RAD-Doom**: 50Hz flicker fusion for color mixing
**Our Version**: Could alternate ASCII patterns for "gray" effects

### 5. Performance Target
**RAD-Doom**: Stable 50 FPS
**Our Version**: Whatever the SIM can handle (probably 1-5 FPS)

## What We CAN'T Use

❌ **RGB Framebuffer** - No memory for this
❌ **External processor** - Must run on SIM itself  
❌ **Floating point** - No FPU on SIM cards
❌ **Full Doom engine** - Still too big
❌ **Audio streaming** - No SID chip!

## The Clever Bits We're Adapting

### From RAD-Doom's multicolor.jpg:
```
RAD: 4 colors per block → Us: 1 ASCII per character
RAD: 160x200 pixels    → Us: 40x25 characters
RAD: VIC-II palette    → Us: ASCII brightness
```

### Memory Comparison:
```
RAD-Doom:  320x200x3 = 192KB framebuffer
Our Text:   40x25x1 = 1KB screen buffer
            192x smaller!
```

## Practical Implementation

I've created `src/doom/rad_inspired.c` that shows:
- Dithered ASCII rendering
- Block coherence enforcement  
- Temporal effects (if useful)
- All in ~2KB of code!

## Try It Yourself

1. Look at the RAD-inspired techniques:
   ```c
   // In rad_inspired.c
   char brightness_to_ascii_dithered(brightness, x, y)
   ```

2. See how we enforce "color" consistency:
   ```c
   void enforce_block_constraints(screen, width, height)
   ```

3. Notice the frame alternation for effects:
   ```c
   // Temporal dithering like RAD-Doom
   uint8_t temporal_offset = (frame_counter & 1) ? 32 : 0;
   ```

## The Bottom Line

RAD-Doom proves that even with a FULL Raspberry Pi, making Doom look good on retro hardware is hard. But their techniques for:
- Color reduction
- Dithering
- Block constraints
- Stable frame rates

...are EXACTLY what we need for SIM cards, just at a much simpler scale!

## Integration Plan

1. Add dithering to our renderer ✓
2. Implement block constraints ✓
3. Test temporal effects (optional)
4. Keep it under 8KB ✓

The code in `rad_inspired.c` shows how to use these RAD-Doom concepts in our ultra-constrained environment!
