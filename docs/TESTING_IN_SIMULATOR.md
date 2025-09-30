# Testing Text Doom in SIM Simulators

## Option 1: Using swSIM (Already in Project)

swSIM is a software-only SIM card simulator included in `tools/swsim/`. It doesn't require any hardware!

### Building swSIM

```bash
cd tools/swsim
make main-dbg
```

### Running swSIM with Text Doom

1. First, we need to integrate our Text Doom APDU handler with swSIM
2. Start the swICC PC/SC interface:
   ```bash
   # Install swICC PC/SC reader (separate project)
   git clone https://github.com/tomasz-lisowski/swicc-pcsc
   cd swicc-pcsc
   make
   ./build/swicc_pcsc
   ```

3. Run swSIM:
   ```bash
   ./build/swsim.elf --ip 127.0.0.1 --port 37324 --fs filesystem.swiccfs
   ```

4. Use our host client to communicate:
   ```bash
   ./build/text_doom_host
   ```

## Option 2: jCardSim (Java Card Simulator)

If you need to test JavaCard deployment:

```bash
# Download jCardSim
wget https://github.com/licel/jcardsim/releases/latest/download/jcardsim.jar

# You'd need to port the C code to Java first
# Then run: java -jar jcardsim.jar
```

## Option 3: Simple Test Harness

For quick testing without full SIM simulation:

```c
// test_sim.c - Simple test harness
#include <stdio.h>
#include "../src/sim/sim_game_main.c"

int main() {
    uint8_t cmd[256];
    uint8_t resp[1024];
    uint16_t resp_len;
    
    // Test INIT_GAME
    cmd[0] = 0x80; cmd[1] = 0x01; cmd[2] = 0x00; cmd[3] = 0x00;
    process_apdu(cmd, 4, resp, &resp_len);
    printf("Init response: %02X %02X\n", resp[0], resp[1]);
    
    // Test SEND_INPUT (move forward)
    cmd[0] = 0x80; cmd[1] = 0x02; cmd[2] = 0x00; cmd[3] = 0x00; 
    cmd[4] = 0x01; cmd[5] = 'w';
    process_apdu(cmd, 6, resp, &resp_len);
    
    // Test GET_SCREEN
    cmd[0] = 0x80; cmd[1] = 0x04; cmd[2] = 0x00; cmd[3] = 0x00;
    process_apdu(cmd, 4, resp, &resp_len);
    
    // Display screen
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 40; x++) {
            printf("%c", resp[y * 40 + x]);
        }
        printf("\n");
    }
    
    return 0;
}
```

Build and run:
```bash
gcc -o test_sim test_sim.c
./test_sim
```

## Option 4: PCSC-Lite Virtual Reader

Create a virtual smart card reader:

```bash
# Install vsmartcard
git clone https://github.com/frankmorgner/vsmartcard
cd vsmartcard/virtualsmartcard
./configure && make && sudo make install

# Run virtual reader
pcscd -f -d
vicc --type relay
```

## Option 5: Online SIM Simulators

- **CosmosEx**: Online JavaCard simulator
- **GlobalPlatformPro**: Has simulation features

## Recommended Approach

For Text Doom testing:

1. **Quick Test**: Use the simple test harness (Option 3)
2. **Full Test**: Set up swSIM (Option 1) 
3. **Production**: Test on real MULTOS/JavaCard hardware

## Integration with swSIM

To integrate Text Doom with swSIM, modify `tools/swsim/src/apduh.c`:

```c
// Add Text Doom command handling
if (apdu->cla == 0x80) {  // DOOM class
    // Call our process_apdu function
    extern void process_apdu(const uint8_t*, uint16_t, uint8_t*, uint16_t*);
    process_apdu(apdu->cmd, apdu->cmd_len, apdu->resp, &apdu->resp_len);
    return;
}
```

Then rebuild swSIM with our game included:
```bash
cd tools/swsim
make EXTRA_SRC="../../src/sim/sim_game_main.c"
```
