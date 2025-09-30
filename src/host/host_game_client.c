/*
 * Host Client for Text Doom on SIM Card
 * Communicates with SIM card via APDU commands
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Non-blocking input for Linux
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

char getch() {
    return getchar();
}
#endif

// Game constants (must match SIM version)
#define SCREEN_W 40
#define SCREEN_H 25

// APDU Commands
#define CLA_DOOM            0x80
#define INS_INIT_GAME       0x01
#define INS_PROCESS_INPUT   0x02
#define INS_UPDATE_GAME     0x03
#define INS_GET_SCREEN      0x04
#define INS_GET_STATUS      0x05

// Simulated SIM card communication
// In real implementation, this would use PC/SC
bool sim_send_apdu(const uint8_t* cmd, uint16_t cmd_len, 
                   uint8_t* resp, uint16_t* resp_len) {
    printf("APDU> ");
    for (uint16_t i = 0; i < cmd_len; i++) {
        printf("%02X ", cmd[i]);
    }
    printf("\n");
    
    // For demo purposes, simulate success
    resp[0] = 0x90;
    resp[1] = 0x00;
    *resp_len = 2;
    return true;
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void display_screen(const uint8_t* screen) {
    clear_screen();
    
    printf("=== TEXT DOOM on SIM Card ===\n\n");
    
    // Draw border
    printf("+");
    for (int i = 0; i < SCREEN_W; i++) printf("-");
    printf("+\n");
    
    // Draw screen
    for (int y = 0; y < SCREEN_H; y++) {
        printf("|");
        for (int x = 0; x < SCREEN_W; x++) {
            printf("%c", screen[y * SCREEN_W + x]);
        }
        printf("|\n");
    }
    
    // Draw bottom border
    printf("+");
    for (int i = 0; i < SCREEN_W; i++) printf("-");
    printf("+\n");
}

bool init_game_on_sim() {
    uint8_t cmd[] = {CLA_DOOM, INS_INIT_GAME, 0x00, 0x00};
    uint8_t resp[256];
    uint16_t resp_len;
    
    if (!sim_send_apdu(cmd, sizeof(cmd), resp, &resp_len)) {
        return false;
    }
    
    return resp[0] == 0x90 && resp[1] == 0x00;
}

bool send_input_to_sim(char input) {
    uint8_t cmd[] = {CLA_DOOM, INS_PROCESS_INPUT, 0x00, 0x00, 0x01, (uint8_t)input};
    uint8_t resp[256];
    uint16_t resp_len;
    
    return sim_send_apdu(cmd, sizeof(cmd), resp, &resp_len);
}

bool update_game_on_sim() {
    uint8_t cmd[] = {CLA_DOOM, INS_UPDATE_GAME, 0x00, 0x00};
    uint8_t resp[256];
    uint16_t resp_len;
    
    return sim_send_apdu(cmd, sizeof(cmd), resp, &resp_len);
}

bool get_screen_from_sim(uint8_t* screen) {
    uint8_t cmd[] = {CLA_DOOM, INS_GET_SCREEN, 0x00, 0x00, 0x00};
    uint8_t resp[1024];
    uint16_t resp_len;
    
    if (!sim_send_apdu(cmd, sizeof(cmd), resp, &resp_len)) {
        return false;
    }
    
    if (resp_len >= SCREEN_W * SCREEN_H + 2) {
        memcpy(screen, resp, SCREEN_W * SCREEN_H);
        return true;
    }
    
    return false;
}

bool get_status_from_sim(uint8_t* health, uint8_t* ammo, uint8_t* level, 
                        bool* game_over, bool* victory) {
    uint8_t cmd[] = {CLA_DOOM, INS_GET_STATUS, 0x00, 0x00, 0x00};
    uint8_t resp[256];
    uint16_t resp_len;
    
    if (!sim_send_apdu(cmd, sizeof(cmd), resp, &resp_len)) {
        return false;
    }
    
    if (resp_len >= 7) {
        *health = resp[0];
        *ammo = resp[1];
        *level = resp[2];
        *game_over = resp[3] != 0;
        *victory = resp[4] != 0;
        return true;
    }
    
    return false;
}

int main(int argc, char* argv[]) {
    bool test_mode = (argc > 1 && strcmp(argv[1], "--test") == 0);
    
    printf("=== TEXT DOOM - SIM Card Host Client ===\n\n");
    
    if (test_mode) {
        printf("Running in TEST MODE (simulated SIM card)\n\n");
    } else {
        printf("Would connect to real SIM card via PC/SC\n");
        printf("Run with --test for demo mode\n");
        return 0;
    }
    
    printf("This client communicates with a SIM card running Doom!\n");
    printf("The entire game runs on the SIM card processor.\n");
    printf("This host just displays the screen and sends input.\n\n");
    
    printf("Controls:\n");
    printf("  W/A/S/D = Move\n");
    printf("  Q/E = Turn\n");
    printf("  SPACE = Fire\n");
    printf("  ESC = Quit\n\n");
    
    printf("Press any key to start...\n");
    getchar();
    
    // Initialize game on SIM
    printf("Initializing game on SIM card...\n");
    if (!init_game_on_sim()) {
        printf("Failed to initialize game!\n");
        return 1;
    }
    
    // Create screen buffer
    uint8_t* screen = malloc(SCREEN_W * SCREEN_H);
    if (!screen) {
        printf("Failed to allocate screen buffer!\n");
        return 1;
    }
    
    // Game loop
    bool running = true;
    
    while (running) {
        // Update game on SIM
        if (!update_game_on_sim()) {
            printf("Failed to update game!\n");
            break;
        }
        
        // Get screen from SIM
        if (!get_screen_from_sim(screen)) {
            printf("Failed to get screen!\n");
            break;
        }
        
        // Display
        display_screen(screen);
        
        // Get status
        uint8_t health, ammo, level;
        bool game_over, victory;
        if (get_status_from_sim(&health, &ammo, &level, &game_over, &victory)) {
            printf("\nStatus: Health=%d Ammo=%d Level=%d", health, ammo, level);
            if (game_over) {
                printf(" - %s!", victory ? "VICTORY" : "GAME OVER");
            }
            printf("\n");
        }
        
        // Check for input
        if (kbhit()) {
            char input = getch();
            
            if (input == 27) {  // ESC
                running = false;
            } else {
                // Send input to SIM
                send_input_to_sim(input);
            }
        }
        
        // Frame rate limiting
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif
    }
    
    // Cleanup
    free(screen);
    printf("\nThanks for playing TEXT DOOM on a SIM card!\n");
    printf("The entire game logic ran on the SIM card processor.\n");
    
    return 0;
}
