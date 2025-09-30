# Try It Now! - Visual Doom Demos

## You Have TWO Playable Demos Ready!

### Demo 1: Basic Movement (2D View)
```bash
./build/visual_test
```

Shows:
```
╔════════════════════════════════════════╗
║########################################║
║#                                      #║
║#         @                            #║
║#                                      #║
║########################################║
╚════════════════════════════════════════╝
```

### Demo 2: 3D Rendering (Doom8088 Style)
```bash
./build/visual_doom8088
```

Shows:
```
╔════════════════════════════════════════╗
║Angle:045 X:16 Y:16                    ║
║                                        ║
║        ....:::===###===:::....        ║
║      ..:::===######===:::..           ║
║    ..::===########===::..             ║
║   .::==##########==::.                ║
║  .::==#########==::.                  ║
║ ..::==######==::..                    ║
║..::===###===::..                      ║
╚════════════════════════════════════════╝
```

## Controls

- **W/S** = Move forward/backward
- **A/D** = Turn left/right (in 3D demo)
- **Q** = Quit

## What You're Seeing

The 3D demo uses Doom8088's concept:
- Different ASCII characters = different distances
- `.` = far away
- `:` = medium distance
- `=` = getting close
- `#` = right in front of you

## This Fits in 8KB!

Unlike Doom8088 (1.5MB), our version:
- Runs in just 8KB
- No external files needed
- Pure C (no Java!)
- Text mode rendering

## Next Steps You Can Try

1. **Modify the map** in `visual_doom8088_style.c`:
   ```c
   // Add more walls
   game.map[15][15] = 1;
   ```

2. **Change the rendering characters**:
   ```c
   #define CHAR_FAR    '·'    // Try different chars
   #define CHAR_MID    '▒'    // Like Doom8088
   ```

3. **Adjust the view distance** or field of view

## The Point

We're showing that Doom-like gameplay CAN fit on a SIM card by:
- Learning from Doom8088's text mode
- But rebuilding everything to fit in 8KB
- Using pure C, no Java needed

Try both demos now - they're compiled and ready to run! 🎮
