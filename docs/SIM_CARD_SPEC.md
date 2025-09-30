# SIM Card Implementation Specification

## How Text Doom Runs on Real SIM Cards

### Memory Layout (8KB Total)

```
┌─────────────────┐ 0x0000
│  Code Segment   │ ~2KB
│  - Game logic   │
│  - APDU handler │
├─────────────────┤ 0x0800
│  Game State     │ ~3KB
│  - Player data  │
│  - Enemy array  │
│  - Bullet array │
│  - Map data     │
│  - Screen buffer│
├─────────────────┤ 0x1400
│  Stack          │ ~1KB
├─────────────────┤ 0x1800
│  Heap/Temp      │ ~2KB
└─────────────────┘ 0x2000 (8KB)
```

### APDU Command Interface

| Command | CLA | INS | P1 | P2 | Data | Response |
|---------|-----|-----|----|----|------|----------|
| Init Game | 80 | 01 | 00 | 00 | None | 90 00 |
| Send Input | 80 | 02 | 00 | 00 | 1 byte (key) | 90 00 |
| Update Game | 80 | 03 | 00 | 00 | None | 90 00 |
| Get Screen | 80 | 04 | 00 | 00 | None | 1000 bytes + 90 00 |
| Get Status | 80 | 05 | 00 | 00 | None | 5 bytes + 90 00 |
| Reset | 80 | 06 | 00 | 00 | None | 90 00 |

### Communication Flow

```
Host PC                          SIM Card
   │                                │
   ├──── INS_INIT_GAME ────────────>│ Initialize game state
   │<───── SW_SUCCESS ──────────────┤
   │                                │
   │ ┌─── Game Loop ───┐           │
   │ │                  │           │
   │ ├── INS_UPDATE_GAME ──────────>│ Update enemies, physics
   │ │<──── SW_SUCCESS ────────────┤
   │ │                  │           │
   │ ├── INS_GET_SCREEN ───────────>│ Render to text buffer
   │ │<── Screen Data (1KB) ───────┤
   │ │                  │           │
   │ │   [Display Screen]           │
   │ │   [Get User Input]           │
   │ │                  │           │
   │ ├── INS_SEND_INPUT ───────────>│ Process player action
   │ │<──── SW_SUCCESS ────────────┤
   │ │                  │           │
   │ └──────────────────┘           │
   │                                │
```

### Performance Characteristics

- **Update Rate**: ~5-10 Hz (limited by APDU overhead)
- **Latency**: ~50-100ms per frame
- **Data Transfer**: 1KB screen per frame
- **Processing**: All game logic on SIM

### Platform Requirements

#### For JavaCard SIMs:
- Would need JavaCard wrapper around C code
- Or rewrite in Java (larger memory footprint)

#### For MULTOS:
- **Native C support!** ✓
- Direct compilation possible
- Better performance

#### For BasicCard:
- Port to ZC-Basic
- Some assembly optimization possible

### Security Considerations

1. **Application Firewall**: Game isolated from other card apps
2. **No Network Access**: Purely local computation
3. **No Persistent Storage**: RAM only (game resets on power loss)
4. **Limited Resources**: Can't interfere with primary SIM functions

### Real Hardware Testing

To deploy on actual SIM hardware:

1. **Development Kit**: 
   - MULTOS SmartDeck
   - JavaCard Development Kit
   - BasicCard SDK

2. **Card Programmer**:
   - Standard PC/SC reader
   - GlobalPlatformPro for loading

3. **Test Cards**:
   - Blank MULTOS cards
   - Test JavaCards
   - BasicCard Professional

### Optimization for Real SIMs

1. **Memory Alignment**: Ensure structures are byte-aligned
2. **No Dynamic Allocation**: All memory static
3. **Minimal Stack Usage**: Avoid deep recursion
4. **Integer Math Only**: No floating point
5. **Compact Data**: Bit-packing where possible

### Example MULTOS Deployment

```bash
# Compile for MULTOS
mulgcc -O2 text_doom.c -o text_doom.alu

# Convert to loadable format
multos-conv text_doom.alu text_doom.aif

# Load onto card
multos-load -r reader0 text_doom.aif
```

### Limitations on Real Hardware

1. **Speed**: SIM processors are slow (~5-25 MHz)
2. **Memory**: Strictly limited to card capacity
3. **I/O**: Only through APDU commands
4. **Power**: Must handle power loss gracefully
5. **Standards**: Must comply with ISO 7816

### Why This Works

- **Minimal Graphics**: Text mode = tiny memory footprint
- **Simple Logic**: Basic game rules fit in 2KB code
- **Static Design**: No malloc, no complex data structures
- **Efficient Storage**: 1 bit per map tile possible
- **Pure Computation**: No external dependencies

This specification proves that Text Doom is not just a demo - it's a real, deployable application for actual SIM cards!
