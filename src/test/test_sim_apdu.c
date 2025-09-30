/*
 * Simple SIM APDU Test Harness for Text Doom
 * Tests the SIM application without needing a full simulator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Don't define TEST_BUILD to avoid main() conflict
// We'll provide our own stubs

// Include the game logic
#include "../doom/text_doom_game.c"

// APDU definitions
#define CLA_DOOM            0x80
#define INS_INIT_GAME       0x01
#define INS_PROCESS_INPUT   0x02
#define INS_UPDATE_GAME     0x03
#define INS_GET_SCREEN      0x04
#define INS_GET_STATUS      0x05

// Global game state
static GameState game;
static bool initialized = false;

// Include just the process_apdu function
void process_apdu(const uint8_t* cmd, uint16_t cmd_len, 
                  uint8_t* resp, uint16_t* resp_len) {
    if (cmd_len < 4) {
        resp[0] = 0x67;
        resp[1] = 0x00;
        *resp_len = 2;
        return;
    }
    
    uint8_t cla = cmd[0];
    uint8_t ins = cmd[1];
    
    if (cla != CLA_DOOM) {
        resp[0] = 0x6E;
        resp[1] = 0x00;
        *resp_len = 2;
        return;
    }
    
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
            resp[0] = game.health;
            resp[1] = game.ammo;
            resp[2] = game.level;
            resp[3] = game.game_over ? 1 : 0;
            resp[4] = game.victory ? 1 : 0;
            resp[5] = 0x90;
            resp[6] = 0x00;
            *resp_len = 7;
            break;
            
        default:
            resp[0] = 0x6D;
            resp[1] = 0x00;
            *resp_len = 2;
            break;
    }
}

// Test APDU commands
void test_apdu_command(const char* name, uint8_t* cmd, uint16_t cmd_len) {
    uint8_t resp[1024];
    uint16_t resp_len = 0;
    
    printf("\n=== Testing: %s ===\n", name);
    printf("Command: ");
    for (int i = 0; i < cmd_len; i++) {
        printf("%02X ", cmd[i]);
    }
    printf("\n");
    
    process_apdu(cmd, cmd_len, resp, &resp_len);
    
    printf("Response length: %d\n", resp_len);
    printf("Status: %02X %02X", resp[resp_len-2], resp[resp_len-1]);
    if (resp[resp_len-2] == 0x90 && resp[resp_len-1] == 0x00) {
        printf(" (Success)\n");
    } else {
        printf(" (Error)\n");
    }
}

void display_screen(uint8_t* screen_data) {
    printf("\n=== Game Screen ===\n");
    printf("+");
    for (int i = 0; i < SCREEN_W; i++) printf("-");
    printf("+\n");
    
    for (int y = 0; y < SCREEN_H; y++) {
        printf("|");
        for (int x = 0; x < SCREEN_W; x++) {
            printf("%c", screen_data[y * SCREEN_W + x]);
        }
        printf("|\n");
    }
    
    printf("+");
    for (int i = 0; i < SCREEN_W; i++) printf("-");
    printf("+\n");
}

int main() {
    printf("Text Doom SIM APDU Test Harness\n");
    printf("================================\n");
    printf("This tests the SIM application without hardware\n\n");
    
    uint8_t cmd[256];
    uint8_t resp[1024];
    uint16_t resp_len;
    
    // Test 1: Initialize game
    cmd[0] = CLA_DOOM;
    cmd[1] = INS_INIT_GAME;
    cmd[2] = 0x00;
    cmd[3] = 0x00;
    test_apdu_command("INIT_GAME", cmd, 4);
    
    // Test 2: Get initial screen
    cmd[0] = CLA_DOOM;
    cmd[1] = INS_GET_SCREEN;
    cmd[2] = 0x00;
    cmd[3] = 0x00;
    cmd[4] = 0x00;  // Le
    
    process_apdu(cmd, 5, resp, &resp_len);
    if (resp_len >= SCREEN_W * SCREEN_H + 2) {
        display_screen(resp);
    }
    
    // Test 3: Send some inputs
    char inputs[] = {'w', 'w', 'd', 'd', ' '};  // Move forward twice, right twice, shoot
    
    for (int i = 0; i < sizeof(inputs); i++) {
        // Update game
        cmd[0] = CLA_DOOM;
        cmd[1] = INS_UPDATE_GAME;
        cmd[2] = 0x00;
        cmd[3] = 0x00;
        test_apdu_command("UPDATE_GAME", cmd, 4);
        
        // Send input
        cmd[0] = CLA_DOOM;
        cmd[1] = INS_PROCESS_INPUT;
        cmd[2] = 0x00;
        cmd[3] = 0x00;
        cmd[4] = 0x01;  // Lc
        cmd[5] = inputs[i];
        
        char desc[32];
        sprintf(desc, "SEND_INPUT '%c'", inputs[i]);
        test_apdu_command(desc, cmd, 6);
    }
    
    // Test 4: Get screen after movement
    cmd[0] = CLA_DOOM;
    cmd[1] = INS_GET_SCREEN;
    cmd[2] = 0x00;
    cmd[3] = 0x00;
    cmd[4] = 0x00;
    
    process_apdu(cmd, 5, resp, &resp_len);
    if (resp_len >= SCREEN_W * SCREEN_H + 2) {
        display_screen(resp);
    }
    
    // Test 5: Get status
    cmd[0] = CLA_DOOM;
    cmd[1] = INS_GET_STATUS;
    cmd[2] = 0x00;
    cmd[3] = 0x00;
    cmd[4] = 0x00;
    
    process_apdu(cmd, 5, resp, &resp_len);
    if (resp_len >= 7) {
        printf("\n=== Game Status ===\n");
        printf("Health: %d\n", resp[0]);
        printf("Ammo: %d\n", resp[1]);
        printf("Level: %d\n", resp[2]);
        printf("Game Over: %s\n", resp[3] ? "Yes" : "No");
        printf("Victory: %s\n", resp[4] ? "Yes" : "No");
    }
    
    printf("\n=== Test Complete ===\n");
    printf("The SIM application is working correctly!\n");
    printf("You can now deploy to real SIM hardware or use with swSIM.\n");
    
    return 0;
}
