# Text Doom APDU Command Reference

## Quick Reference

| Command | CLA | INS | P1 | P2 | Data | Response | Description |
|---------|-----|-----|----|----|------|----------|-------------|
| Init Game | 80 | 01 | 00 | 00 | - | 90 00 | Initialize new game |
| Send Input | 80 | 02 | 00 | 00 | 1 byte | 90 00 | Send key press |
| Update Game | 80 | 03 | 00 | 00 | - | 90 00 | Process one game tick |
| Get Screen | 80 | 04 | 00 | 00 | - | 1000 bytes + 90 00 | Get 40x25 display |
| Get Status | 80 | 05 | 00 | 00 | - | 5 bytes + 90 00 | Get game status |
| Reset | 80 | 06 | 00 | 00 | - | 90 00 | Reset game |

## Detailed Commands

### INIT_GAME (CLA=80 INS=01)
Initializes a new game with player at starting position.

**Command**: `80 01 00 00`  
**Response**: `90 00` (success)

### SEND_INPUT (CLA=80 INS=02)
Sends a single keystroke to the game.

**Command**: `80 02 00 00 01 [key]`  
**Data byte values**:
- `77` (w) - Move forward
- `73` (s) - Move backward  
- `61` (a) - Strafe left
- `64` (d) - Strafe right
- `71` (q) - Turn left
- `65` (e) - Turn right
- `20` (space) - Fire
- `72` (r) - Restart (when dead)

**Response**: `90 00` (success)

### UPDATE_GAME (CLA=80 INS=03)
Updates game state (moves enemies, processes bullets).

**Command**: `80 03 00 00`  
**Response**: `90 00` (success)

### GET_SCREEN (CLA=80 INS=04)
Retrieves the current screen display.

**Command**: `80 04 00 00 00`  
**Response**: 1000 bytes (40x25 ASCII) + `90 00`

### GET_STATUS (CLA=80 INS=05)
Gets current game status.

**Command**: `80 05 00 00 00`  
**Response**: 5 bytes + `90 00`
- Byte 0: Health (0-100)
- Byte 1: Ammo (0-99)
- Byte 2: Level (1+)
- Byte 3: Game Over (0/1)
- Byte 4: Victory (0/1)

### RESET_GAME (CLA=80 INS=06)
Resets the game to initial state.

**Command**: `80 06 00 00`  
**Response**: `90 00` (success)

## Error Codes

| SW1 SW2 | Meaning |
|---------|---------|
| 90 00 | Success |
| 67 00 | Wrong length |
| 69 86 | Command not allowed (not initialized) |
| 6D 00 | Invalid instruction |
| 6E 00 | Invalid class |

## Example Session

```
// Initialize game
>> 80 01 00 00
<< 90 00

// Move forward
>> 80 02 00 00 01 77
<< 90 00

// Update game
>> 80 03 00 00
<< 90 00

// Get screen
>> 80 04 00 00 00
<< [1000 bytes of screen data] 90 00

// Check status
>> 80 05 00 00 00
<< 64 14 01 00 00 90 00  (Health=100, Ammo=20, Level=1, Not game over, Not victory)
```

## Testing Without Hardware

Use the included test harness:
```bash
./build/test_sim_apdu
```

This simulates the APDU interface without needing a real SIM card.
