/*
 * Playable Text Doom - Test the complete game logic
 * Pure ASCII, no color, no sound
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

// Include the game logic
#include "../doom/text_doom_game.c"

// Platform-specific functions
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#ifndef _WIN32
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

// Display the game screen
void display_game(GameState* game) {
    clear_screen();
    
    printf("=== TEXT DOOM - Pure ASCII Gaming ===\n");
    printf("Memory used: ~%zu bytes (fits in 8KB!)\n\n", sizeof(GameState));
    
    // Draw border
    printf("+");
    for (int i = 0; i < SCREEN_W; i++) printf("-");
    printf("+\n");
    
    // Draw game screen
    for (int y = 0; y < SCREEN_H; y++) {
        printf("|");
        for (int x = 0; x < SCREEN_W; x++) {
            printf("%c", game->screen[y][x]);
        }
        printf("|\n");
    }
    
    // Draw bottom border
    printf("+");
    for (int i = 0; i < SCREEN_W; i++) printf("-");
    printf("+\n");
    
    // Legend
    printf("\nLEGEND: @ = You, E = Enemy, * = Bullet\n");
    printf("        # = Wall, X = Exit, a = Ammo, + = Health\n");
    printf("        ^ > v < = Your facing direction\n");
    
    if (game->game_over) {
        printf("\nPress 'R' to restart or 'Q' to quit\n");
    }
}

// Main game loop
int main() {
    GameState* game = malloc(sizeof(GameState));
    if (!game) {
        printf("Failed to allocate game state!\n");
        return 1;
    }
    
    printf("=== TEXT DOOM ===\n");
    printf("A complete Doom-like game in pure ASCII\n");
    printf("Fits in 8KB of memory!\n\n");
    printf("Controls:\n");
    printf("  W/A/S/D = Move\n");
    printf("  Q/E = Turn left/right\n");
    printf("  SPACE = Fire\n");
    printf("  ESC = Quit\n\n");
    printf("Find the exit (X) while avoiding enemies (E)!\n");
    printf("Collect ammo (a) and health (+) to survive!\n\n");
    printf("Press any key to start...\n");
    
    getch();
    
    // Initialize game
    init_game(game);
    
    // Game loop
    bool running = true;
    int frame_counter = 0;
    
    while (running) {
        // Update game logic
        update_game(game);
        
        // Render
        render_game(game);
        display_game(game);
        
        // Check for input (non-blocking)
        if (kbhit()) {
            char input = getch();
            
            if (input == 27 || input == 'Q') {  // ESC or Q to quit
                if (game->game_over || input == 27) {
                    running = false;
                } else {
                    turn_player(game, -1);  // Q turns left in-game
                }
            } else {
                process_game_input(game, input);
            }
        }
        
        // Frame rate limiting (approximately 10 FPS)
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif
        
        frame_counter++;
    }
    
    // Cleanup
    clear_screen();
    printf("Thanks for playing TEXT DOOM!\n");
    printf("This entire game fits in 8KB - perfect for SIM cards!\n");
    printf("\nGame Statistics:\n");
    printf("- Memory used: %zu bytes\n", sizeof(GameState));
    printf("- Frames played: %d\n", frame_counter);
    printf("- Final score: Health=%d, Ammo=%d, Level=%d\n", 
           game->health, game->ammo, game->level);
    
    free(game);
    return 0;
}

// Summary of what we've implemented:
// 1. Complete game loop with update/render cycle
// 2. Player movement with collision detection
// 3. Shooting mechanics with projectiles
// 4. Enemy AI that follows the player
// 5. Pickups (health and ammo)
// 6. Win condition (reach exit)
// 7. Lose condition (health reaches 0)
// 8. All rendered in pure ASCII text
// 9. No sound, no color - just gameplay
// 10. Entire game state fits in ~3KB!
