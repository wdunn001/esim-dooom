# PLAY TEXT DOOM NOW! 🎮

## Quick Start:

```bash
# Build and play immediately:
make play && ./build/play_text_doom
```

## Controls:

| Key | Action |
|-----|--------|
| W | Move Forward |
| S | Move Backward |
| A | Strafe Left |
| D | Strafe Right |
| Q | Turn Left |
| E | Turn Right |
| SPACE | Fire! |
| R | Restart (when dead) |
| ESC | Quit |

## What You See:

```
+----------------------------------------+
|########################################|
|#                 ^                    #|
|#                 @      E             #|
|#       a                   *          #|
|#                                      #|
|#              +                    X  #|
|########################################|
|HP:085 AM:15 L:1                       |
|WASD=move QE=turn SPC=fire             |
+----------------------------------------+
```

## Legend:

- `@` = You
- `^ > v <` = Direction indicator
- `E` = Enemy (shoots back!)
- `*` = Bullet in flight
- `#` = Wall (can't pass)
- `X` = Exit (win the level!)
- `a` = Ammo pickup (+10)
- `+` = Health pickup (+25)

## Tips:

1. **Conserve Ammo** - You start with only 20 shots
2. **Keep Moving** - Enemies always move toward you
3. **Find Pickups** - Look for 'a' and '+' symbols
4. **Reach the Exit** - The 'X' wins the level

## Features:

- **Real Combat**: Bullets travel, enemies dodge
- **Resource Management**: Limited ammo and health
- **Collision Detection**: Can't walk through walls
- **AI Enemies**: They hunt you down!
- **Multiple Hits**: Enemies take 2 shots to kill

## Technical Marvel:

This entire game, including:
- Game logic
- Rendering engine  
- Enemy AI
- Physics
- Map data
- All game state

**Fits in less than 8KB!** Perfect for SIM cards!

## Variations to Try:

```bash
# Original demos:
./build/visual_test      # 2D movement
./build/visual_doom8088  # 3D ASCII view

# Full game:
./build/play_text_doom   # Complete game!
```

## Have Fun!

This is Doom stripped to its absolute essence - pure gameplay in pure text. No graphics, no sound, just you vs enemies in ASCII!

**Can you find the exit before running out of health?** 🎯
