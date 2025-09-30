# RAD-Doom Enhanced Edition

## Overview

RAD-Doom Enhanced is a special version of Text Doom that leverages the extra memory available in modern eSIM cards (32KB-64KB RAM). It's inspired by the RAD-Doom project for Commodore 64, which used clever rendering techniques to achieve impressive visuals.

## Features

### Visual Enhancements
- **ANSI Color Support**: Full color rendering using terminal escape codes
- **Dithering Effects**: Text-based dithering patterns inspired by RAD-Doom
- **Particle System**: Visual effects for explosions and impacts
- **Enhanced HUD**: Color-coded health bars and status indicators

### Gameplay Enhancements (64KB+ config)
- **Larger Maps**: 60x40 instead of 20x20
- **More Enemies**: Up to 20 simultaneous enemies
- **Multiple Weapons**: 4 different weapon types
- **Advanced AI**: Smarter enemy behavior
- **Save States**: Game progress can be saved (future feature)

## Building RAD-Doom Enhanced

```bash
# Build the enhanced version
make rad-doom

# Or build all enhanced versions
make enhanced
```

## Running

```bash
./build/play_rad_doom
```

## Visual Effects

### Color Coding
- 🔴 **Red**: Enemies and damage
- 🟢 **Green**: Player and health pickups
- 🟡 **Yellow**: Bullets and ammo
- 🔵 **Blue**: Floor details
- 🟣 **Magenta**: Level exit
- ⚪ **White**: Walls with dithering

### Dithering Patterns
The game uses character-based dithering similar to RAD-Doom:
```
  Light ──► Dark
  ' ' . : + # @ %
```

This creates the illusion of different shades using only text characters.

## Memory Usage

| Component | 8KB Version | 64KB Version |
|-----------|-------------|--------------|
| Map Data | 400 bytes | 2,560 bytes |
| Enemies | 24 bytes | 160 bytes |
| Particles | None | 200 bytes |
| Screen Buffer | 1,000 bytes | 3,200 bytes |
| **Total** | ~2KB | ~8KB |

## RAD-Doom Inspiration

The original RAD-Doom for C64 demonstrated:
1. **Color Reduction**: Limited palette used creatively
2. **Dithering**: Mixing colors through patterns
3. **Block Constraints**: Working within character-based limits

Our enhanced version adapts these concepts for text-mode SIM cards:
- Uses ASCII characters as "pixels"
- ANSI colors for visual variety
- Dithering through character patterns
- Maintains playability at high frame rates

## Terminal Requirements

For best results:
- Terminal with ANSI color support
- Minimum 80x50 character display
- Fast terminal emulator (native terminal preferred over SSH)
- Dark background recommended

## Performance

- **8KB Version**: 10-15 FPS on SIM hardware
- **64KB Version**: 5-10 FPS on SIM hardware (more to render)
- **PC Testing**: 20+ FPS easily achieved

## Future Enhancements

With 64KB+ RAM, we could add:
- Sound patterns (beep sequences)
- More particle effects
- Destructible walls
- Multiple levels with transitions
- Mini-map display
- Inventory system

## Comparison

| Feature | Basic (8KB) | Enhanced (64KB) |
|---------|-------------|-----------------|
| Colors | No | Yes (ANSI) |
| Map Size | 20x20 | 60x40 |
| Enemies | 3 | 20 |
| Weapons | 1 | 4 |
| Effects | None | Particles |
| Dithering | No | Yes |

## Try It Now!

```bash
# See the difference yourself
make minimal && ./build/play_text_doom  # Basic version
make rad-doom && ./build/play_rad_doom   # Enhanced version
```

The enhanced version shows what's possible when SIM cards have more memory - bringing us closer to the original Doom experience while staying within the constraints of text-mode rendering!
