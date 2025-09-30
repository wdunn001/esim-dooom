/*
 * Visual Test - Shows the current "screen" output
 * This demonstrates what would be displayed on a host device
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Screen dimensions (must match SIM)
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 25

// Simple game state for testing
typedef struct {
    int player_x;
    int player_y;
    uint8_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];
} TestGame;

static TestGame game;

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void init_game() {
    // Clear screen
    memset(game.screen, ' ', SCREEN_WIDTH * SCREEN_HEIGHT);
    
    // Draw borders
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        game.screen[x] = '#';  // Top wall
        game.screen[(SCREEN_HEIGHT-1) * SCREEN_WIDTH + x] = '#';  // Bottom wall
    }
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        game.screen[y * SCREEN_WIDTH] = '#';  // Left wall
        game.screen[y * SCREEN_WIDTH + SCREEN_WIDTH-1] = '#';  // Right wall
    }
    
    // Place player in center
    game.player_x = SCREEN_WIDTH / 2;
    game.player_y = SCREEN_HEIGHT / 2;
    
    // Add some walls
    for (int i = 5; i < 15; i++) {
        game.screen[10 * SCREEN_WIDTH + i] = '#';
        game.screen[i * SCREEN_WIDTH + 20] = '#';
    }
}

void update_player() {
    // Clear old position
    game.screen[game.player_y * SCREEN_WIDTH + game.player_x] = '.';
    
    // Draw player at new position
    if (game.screen[game.player_y * SCREEN_WIDTH + game.player_x] != '#') {
        game.screen[game.player_y * SCREEN_WIDTH + game.player_x] = '@';
    }
}

void move_player(char input) {
    int new_x = game.player_x;
    int new_y = game.player_y;
    
    switch(input) {
        case 'w': case 'W': new_y--; break;
        case 's': case 'S': new_y++; break;
        case 'a': case 'A': new_x--; break;
        case 'd': case 'D': new_x++; break;
    }
    
    // Check bounds and collision
    if (new_x >= 0 && new_x < SCREEN_WIDTH && 
        new_y >= 0 && new_y < SCREEN_HEIGHT &&
        game.screen[new_y * SCREEN_WIDTH + new_x] != '#') {
        game.player_x = new_x;
        game.player_y = new_y;
    }
}

void display_screen() {
    clear_screen();
    printf("╔");
    for (int i = 0; i < SCREEN_WIDTH; i++) printf("═");
    printf("╗\n");
    
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        printf("║");
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            printf("%c", game.screen[y * SCREEN_WIDTH + x]);
        }
        printf("║\n");
    }
    
    printf("╚");
    for (int i = 0; i < SCREEN_WIDTH; i++) printf("═");
    printf("╝\n");
    
    printf("\nControls: W/A/S/D = Move, Q = Quit\n");
    printf("This is what would display on a SIM card (if it had a screen!)\n");
}

int main(void) {
    printf("eSIM Doom - Visual Demo\n");
    printf("This shows what the SIM card would be 'rendering'\n");
    printf("Press Enter to start...\n");
    getchar();
    
    // Initialize
    init_game();
    update_player();
    
    // Game loop
    char input;
    while (1) {
        // Display current state
        display_screen();
        
        // Get input
        printf("Move: ");
        input = getchar();
        if (input == '\n') continue;  // Skip newlines
        
        if (input == 'q' || input == 'Q') break;
        
        // Process input
        move_player(input);
        update_player();
        
        // Clear input buffer
        while (getchar() != '\n');
    }
    
    printf("Thanks for playing!\n");
    return 0;
}
