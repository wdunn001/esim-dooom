/*
 * SIM Card Text Doom - Main Application
 * Integrates the game logic with SIM card APDU interface
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

// SIM Card constraints
#define SIM_RAM_SIZE 8192

// Include game logic (it defines its own structures)
#include "../doom/text_doom_game.c"

// APDU Commands
#define CLA_DOOM            0x80
#define INS_INIT_GAME       0x01
#define INS_PROCESS_INPUT   0x02
#define INS_UPDATE_GAME     0x03
#define INS_GET_SCREEN      0x04
#define INS_GET_STATUS      0x05
#define INS_RESET_GAME      0x06

// APDU Status words
#define SW_SUCCESS          0x9000
#define SW_WRONG_LENGTH     0x6700
#define SW_WRONG_CLASS      0x6E00
#define SW_WRONG_INS        0x6D00

// Global game state (stored in SIM memory)
static GameState game;
static bool initialized = false;

// Function prototypes for SIM card communication
uint16_t receive_apdu(uint8_t* buffer);
void send_apdu(const uint8_t* buffer, uint16_t len);

// Process APDU commands
void process_apdu(const uint8_t* cmd, uint16_t cmd_len, 
                  uint8_t* resp, uint16_t* resp_len) {
    
    // Check minimum length
    if (cmd_len < 4) {
        resp[0] = 0x67;
        resp[1] = 0x00;
        *resp_len = 2;
        return;
    }
    
    uint8_t cla = cmd[0];
    uint8_t ins = cmd[1];
    // uint8_t p1 = cmd[2];  // Not used yet
    // uint8_t p2 = cmd[3];  // Not used yet
    
    // Check class
    if (cla != CLA_DOOM) {
        resp[0] = 0x6E;
        resp[1] = 0x00;
        *resp_len = 2;
        return;
    }
    
    // Process instruction
    switch (ins) {
        case INS_INIT_GAME:
            init_game(&game);
            initialized = true;
            resp[0] = 0x90;
            resp[1] = 0x00;
            *resp_len = 2;
            break;
            
        case INS_PROCESS_INPUT:
            if (!initialized || cmd_len < 5) {
                resp[0] = 0x67;
                resp[1] = 0x00;
                *resp_len = 2;
                break;
            }
            process_game_input(&game, cmd[4]);
            resp[0] = 0x90;
            resp[1] = 0x00;
            *resp_len = 2;
            break;
            
        case INS_UPDATE_GAME:
            if (!initialized) {
                resp[0] = 0x69;
                resp[1] = 0x86;
                *resp_len = 2;
                break;
            }
            update_game(&game);
            render_game(&game);
            resp[0] = 0x90;
            resp[1] = 0x00;
            *resp_len = 2;
            break;
            
        case INS_GET_SCREEN:
            if (!initialized) {
                resp[0] = 0x69;
                resp[1] = 0x86;
                *resp_len = 2;
                break;
            }
            // Return screen data
            memcpy(resp, game.screen, SCREEN_W * SCREEN_H);
            resp[SCREEN_W * SCREEN_H] = 0x90;
            resp[SCREEN_W * SCREEN_H + 1] = 0x00;
            *resp_len = SCREEN_W * SCREEN_H + 2;
            break;
            
        case INS_GET_STATUS:
            if (!initialized) {
                resp[0] = 0x69;
                resp[1] = 0x86;
                *resp_len = 2;
                break;
            }
            // Return game status
            resp[0] = game.health;
            resp[1] = game.ammo;
            resp[2] = game.level;
            resp[3] = game.game_over ? 1 : 0;
            resp[4] = game.victory ? 1 : 0;
            resp[5] = 0x90;
            resp[6] = 0x00;
            *resp_len = 7;
            break;
            
        case INS_RESET_GAME:
            memset(&game, 0, sizeof(game));
            initialized = false;
            resp[0] = 0x90;
            resp[1] = 0x00;
            *resp_len = 2;
            break;
            
        default:
            resp[0] = 0x6D;
            resp[1] = 0x00;
            *resp_len = 2;
            break;
    }
}

// Main entry point for SIM application
void sim_main(void) {
    uint8_t cmd_buffer[256];
    uint8_t resp_buffer[1024];  // Screen data is 1000 bytes
    uint16_t cmd_len, resp_len;
    
    // Main APDU loop
    while (1) {
        // Wait for APDU command (SIM OS handles this)
        cmd_len = receive_apdu(cmd_buffer);
        
        // Process command
        resp_len = 0;
        process_apdu(cmd_buffer, cmd_len, resp_buffer, &resp_len);
        
        // Send response
        send_apdu(resp_buffer, resp_len);
    }
}

// Stub functions for SIM card communication
// In real implementation, these interface with the card OS
uint16_t receive_apdu(uint8_t* buffer) {
    (void)buffer;
    return 0;  // Stub
}

void send_apdu(const uint8_t* buffer, uint16_t len) {
    (void)buffer;
    (void)len;
    // Stub
}

// Memory usage summary:
// GameState: ~3KB
// Code: ~2KB  
// Stack/misc: ~1KB
// Total: ~6KB - fits comfortably in 8KB SIM memory!

#ifdef TEST_BUILD
// Test main for standalone testing
int main(void) {
    printf("Text Doom SIM Application\n");
    printf("GameState size: %zu bytes\n", sizeof(GameState));
    printf("This would run on a SIM card via APDU commands.\n");
    return 0;
}
#endif
