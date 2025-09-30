# SIM Card and eSIM Development Tools Guide

## Overview

After researching the current landscape, here's what you need to know about developing applications for SIM cards and eSIMs:

## The Reality Check

**PlatformIO** is primarily designed for microcontroller development (Arduino, ESP32, STM32, etc.) and **does not natively support JavaCard/SIM card development**. While there are some community extensions for GSM modules, these are for communicating WITH SIM cards, not programming them directly.

## Actual Tools for SIM Card Development

### 1. JavaCard Development Kit (JCDK)
- **Oracle JavaCard SDK**: The official development kit for JavaCard applets
- Includes compiler, converter, and simulator
- Version 3.0.5 or newer recommended for modern SIM cards
- Download from Oracle (requires registration)

### 2. GlobalPlatformPro
- **GitHub**: martinpaljak/GlobalPlatformPro
- Essential open-source tool for loading applets onto real cards
- Handles authentication and secure channel protocols
- Command-line tool written in Java

### 3. jCardSim
- JavaCard simulator for testing without physical cards
- Useful for initial development and debugging
- Can simulate various JavaCard API versions

### 4. PCSC Tools
- **pcscd**: PC/SC daemon for card reader access
- **pcsc-tools**: Command-line utilities for card interaction
- Essential for testing with physical cards

### 5. Development IDEs
- **Eclipse with JCDE plugin**: Traditional JavaCard development
- **IntelliJ IDEA**: With JavaCard plugin support
- **VS Code**: Can be configured for JavaCard development

## eSIM Specifics

eSIMs are essentially the same JavaCard environment but:
1. Embedded in the device (no physical card)
2. Support remote provisioning via RSP (Remote SIM Provisioning)
3. Use the same JavaCard APIs as physical SIM cards

## Recommended Development Stack

```bash
# Core Tools
1. Oracle JavaCard SDK (JCDK) - For compilation
2. GlobalPlatformPro - For card management
3. jCardSim - For testing
4. PCSC-lite + tools - For hardware access

# Development Environment
- WSL2 Ubuntu for consistent build environment
- Java 8 or 11 (JavaCard compatibility)
- Python for automation scripts
- Git for version control
```

## SIM Card Constraints for Doom8088

### Memory Limitations
- **RAM**: 2-8 KB typical (high-end cards may have 32-64 KB)
- **EEPROM**: 16-256 KB for code and persistent data
- **Transient memory**: Very limited, cleared between sessions

### Processing Constraints
- No floating-point operations
- Limited stack depth
- No dynamic memory allocation
- Single-threaded execution
- APDU-based communication only

### JavaCard API Limitations
- No direct screen access
- No file system (use EEPROM arrays)
- No networking beyond APDU
- Limited math operations
- No standard libraries (no stdlib, no libc)

## Alternative Approaches

Given these constraints, consider:

1. **Minimal Doom Engine**: Strip Doom8088 to absolute minimum
   - Text-based rendering
   - Turn-based gameplay
   - Minimal map data
   - No textures, just walls

2. **Host-Card Split Architecture**:
   - Game logic on SIM card
   - Rendering on host device
   - APDU commands for game state

3. **Proof of Concept First**:
   - Start with "Can we draw a square?"
   - Then "Can we move it?"
   - Build up to basic game mechanics

## Getting Started Commands

```bash
# In WSL2 Ubuntu
cd /mnt/c/Users/willi/esim-dooom

# Install basic tools
sudo apt update
sudo apt install -y \
    openjdk-11-jdk \
    pcscd \
    pcsc-tools \
    libpcsclite-dev \
    git \
    python3 \
    python3-pip

# Install GlobalPlatformPro
wget https://github.com/martinpaljak/GlobalPlatformPro/releases/latest/download/gp.jar
sudo mv gp.jar /usr/local/bin/
echo '#!/bin/bash' | sudo tee /usr/local/bin/gp
echo 'java -jar /usr/local/bin/gp.jar "$@"' | sudo tee -a /usr/local/bin/gp
sudo chmod +x /usr/local/bin/gp

# Test card reader (if connected)
pcsc_scan
```

## Next Steps

1. Download Oracle JavaCard SDK
2. Set up a simple "Hello World" applet
3. Test with jCardSim
4. Analyze Doom8088 code for adaptability
5. Create minimal proof of concept

## Resources

- JavaCard Tutorial: https://github.com/martinpaljak/JavaCardTutorial
- GlobalPlatformPro Wiki: https://github.com/martinpaljak/GlobalPlatformPro/wiki
- JavaCard API Documentation: Oracle's official docs
- Smart Card community: https://www.smartcardfocus.com/

## Important Note

Running Doom on a SIM card is an extreme challenge. Most SIM cards have less computing power than a 1980s calculator. This project will require significant creativity and compromise to achieve even a minimal version of the game.
