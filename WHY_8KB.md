# Why We Target 8KB RAM

## The Reality of SIM Card Memory

When people hear "64KB SIM card", they often think that's all available for apps. **It's not!**

### Memory Breakdown on a "64KB" SIM Card:

```
Total Memory: 64KB
├── ROM: 16-24KB     (Card Operating System)
├── EEPROM: 32-48KB  (Persistent storage - contacts, SMS, etc.)
└── RAM: 8-16KB      (Working memory for apps) ← THIS IS OUR LIMIT!
```

### RAM vs Storage

- **RAM** (Random Access Memory): Where your program runs
- **EEPROM/Flash**: Where data is stored (like a tiny hard drive)

You can't run code from EEPROM - it must be loaded into RAM first!

## SIM Card Types and Their RAM

| Card Type | Total Memory | Actual RAM | What We Can Use |
|-----------|--------------|------------|-----------------|
| Basic SIM | 32KB | 2-4KB | 2KB after OS |
| Standard SIM | 64KB | 8KB | 6KB after OS |
| Java Card Classic | 64-128KB | 8-16KB | 6-12KB |
| Modern USIM | 128-256KB | 32KB | 24KB |
| High-end eSIM | 512KB+ | 64-128KB | 48KB+ |

## Why Default to 8KB?

1. **Compatibility**: Works on the vast majority of SIM cards
2. **Real constraint**: Many cards only have 8KB total RAM
3. **OS overhead**: The card OS uses 2-3KB, leaving ~6KB for apps
4. **Market reality**: Billions of 8KB cards still in use

## But My eSIM Has More!

Yes! Modern eSIMs are much more powerful. That's why we support:

```bash
# For basic SIM cards (8KB RAM)
make minimal

# For modern USIM cards (32KB RAM)  
make standard

# For high-end eSIMs (64KB+ RAM)
make enhanced
```

## Real-World Examples

### Gemalto IDPrime 3810 (Common SIM)
- Advertised: "80KB memory"
- Reality: 8KB RAM, 72KB EEPROM

### G+D SmartCafe (Modern eSIM)
- Advertised: "512KB memory"  
- Reality: 64KB RAM, 448KB Flash

### NXP JCOP (High-end)
- Advertised: "1MB memory"
- Reality: 128KB RAM, 896KB Flash

## The Achievement

Making Doom run in 8KB RAM is like:
- Fitting a modern game in the memory of a 1980s calculator
- Running Windows in the RAM of an Arduino
- Putting a Ferrari engine in a bicycle

That's what makes this project special - it truly runs on the most basic SIM cards!

## Want More Features?

If your card has more RAM:

```bash
# Check what your card supports
make memory-detect && ./build/memory_detect

# Build for your card's capability
make enhanced  # For 64KB+ RAM cards
```

But remember: The 8KB version is the most impressive feat of optimization!
