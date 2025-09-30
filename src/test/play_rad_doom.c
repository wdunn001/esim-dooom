/*
 * Play RAD-Doom Enhanced - Standalone launcher
 * For 32KB+ SIM cards with advanced features
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Build with enhanced configuration
#define USE_CONFIG_HEADER
#define MEMORY_CONFIG 3  // ENHANCED (64KB+)
#include "../doom/doom_config.h"

// Use the enhanced game implementation
#include "../doom/text_doom_game.c"

// Additional RAD-inspired features
#define ENABLE_COLOR 1
#define ENABLE_PARTICLES 1

// ANSI color codes for enhanced visuals
#define COL_RESET   "\033[0m"
#define COL_RED     "\033[31m"
#define COL_GREEN   "\033[32m"
#define COL_YELLOW  "\033[33m"
#define COL_BLUE    "\033[34m"
#define COL_MAGENTA "\033[35m"
#define COL_CYAN    "\033[36m"
#define COL_WHITE   "\033[37m"
#define COL_BRIGHT  "\033[1m"
#define COL_DIM     "\033[2m"

// RAD-style dithering patterns
const char DITHER_CHARS[] = " .:-=+*#%@";

// Enhanced display with color and dithering
void display_rad_screen(const GameState* game) {
    system("clear");
    
    // Title with color
    printf(COL_BRIGHT COL_CYAN "=== RAD-DOOM ENHANCED ===" COL_RESET " ");
    printf(COL_YELLOW "(%s Edition)" COL_RESET "\n\n", MEMORY_SIZE_STR);
    
    // Game screen with dithering effects
    for (int y = 0; y < SCREEN_H; y++) {
        for (int x = 0; x < SCREEN_W; x++) {
            char c = game->screen[y][x];
            
            // Apply color based on character
            if (ENABLE_COLOR) {
                switch (c) {
                    case '#':  // Walls
                        printf(COL_DIM COL_WHITE);
                        // Apply dithering pattern based on position
                        c = DITHER_CHARS[((x + y) % 10)];
                        break;
                    case 'E':  // Enemies
                        printf(COL_BRIGHT COL_RED);
                        break;
                    case '@':  // Player
                        printf(COL_BRIGHT COL_GREEN);
                        break;
                    case '*':  // Bullets
                        printf(COL_BRIGHT COL_YELLOW);
                        break;
                    case '+':  // Health
                        printf(COL_BRIGHT COL_GREEN);
                        break;
                    case 'a':  // Ammo
                        printf(COL_YELLOW);
                        break;
                    case 'X':  // Exit
                        printf(COL_BRIGHT COL_MAGENTA);
                        break;
                    case '.':  // Floor detail
                        printf(COL_DIM COL_BLUE);
                        break;
                    default:
                        printf(COL_RESET);
                        break;
                }
            }
            
            printf("%c", c);
            
            if (ENABLE_COLOR) {
                printf(COL_RESET);
            }
        }
        printf("\n");
    }
    
    // Enhanced HUD with color
    printf("\n");
    printf(COL_BRIGHT "╔════════════════════════════════════════╗\n" COL_RESET);
    printf(COL_BRIGHT "║ " COL_RESET);
    
    // Health bar
    printf(COL_RED "HP:" COL_RESET);
    int hp_bars = game->health / 5;
    for (int i = 0; i < 20; i++) {
        if (i < hp_bars) {
            if (game->health > 60) printf(COL_GREEN);
            else if (game->health > 30) printf(COL_YELLOW);
            else printf(COL_RED);
            printf("█");
        } else {
            printf(COL_DIM "░" COL_RESET);
        }
    }
    printf(COL_RESET " %03d ", game->health);
    
    // Ammo
    printf(COL_YELLOW "AMMO:%02d" COL_RESET " ", game->ammo);
    
    // Level
    printf(COL_CYAN "L:%d" COL_RESET " ", game->level);
    
    printf(COL_BRIGHT "║\n" COL_RESET);
    printf(COL_BRIGHT "╚════════════════════════════════════════╝\n" COL_RESET);
    
    // Advanced features info
    #if HAS_PARTICLE_EFFECTS
    printf(COL_DIM "Particle Effects: ON  " COL_RESET);
    #endif
    #if HAS_ADVANCED_AI
    printf(COL_DIM "Advanced AI: ON  " COL_RESET);
    #endif
    #if HAS_SAVE_STATES
    printf(COL_DIM "Save States: ON" COL_RESET);
    #endif
    printf("\n");
    
    // Controls
    printf(COL_DIM "\nControls: WASD=move QE=turn SPACE=fire");
    #if HAS_MULTIPLE_WEAPONS
    printf(" 1-4=weapons");
    #endif
    printf(" ESC=quit\n" COL_RESET);
}

// Non-blocking input
int kbhit(void) {
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

// Main game loop
int main() {
    GameState game;
    
    // Show configuration
    printf(COL_BRIGHT COL_CYAN);
    printf("╔══════════════════════════════════════════╗\n");
    printf("║       RAD-DOOM ENHANCED EDITION          ║\n");
    printf("║         Memory Config: %s            ║\n", MEMORY_SIZE_STR);
    printf("║      Max Enemies: %2d                     ║\n", MAX_ENEMIES);
    printf("║      Map Size: %2dx%2d                    ║\n", MAP_W, MAP_H);
    printf("╚══════════════════════════════════════════╝\n");
    printf(COL_RESET);
    
    printf("\nThis enhanced version includes:\n");
    printf("- " COL_GREEN "Color rendering" COL_RESET " with ANSI codes\n");
    printf("- " COL_YELLOW "Dithering effects" COL_RESET " inspired by RAD-Doom\n");
    printf("- " COL_CYAN "Larger maps" COL_RESET " (%dx%d instead of 20x20)\n", MAP_W, MAP_H);
    printf("- " COL_MAGENTA "More enemies" COL_RESET " (up to %d)\n", MAX_ENEMIES);
    #if HAS_PARTICLE_EFFECTS
    printf("- " COL_RED "Particle effects" COL_RESET " for explosions\n");
    #endif
    #if HAS_ADVANCED_AI
    printf("- " COL_BLUE "Advanced enemy AI" COL_RESET "\n");
    #endif
    
    printf("\nPress any key to start...");
    getchar();
    
    // Initialize game
    init_game(&game);
    
    // Game loop
    bool running = true;
    int frame = 0;
    
    while (running && !game.game_over) {
        // Display
        display_rad_screen(&game);
        
        // Update game
        update_game(&game);
        render_game(&game);
        
        // Handle input
        if (kbhit()) {
            char key = getchar();
            
            if (key == 27) {  // ESC
                running = false;
            } else if (key == 'r' && game.game_over) {
                init_game(&game);
            } else {
                process_game_input(&game, key);
            }
        }
        
        // Frame rate control (targeting ~20 FPS for smoothness)
        usleep(50000);  // 50ms
        frame++;
        
        // Add some visual effects every few frames
        if (frame % 5 == 0 && ENABLE_PARTICLES) {
            // Simulate particle effects by adding dots to empty spaces
            for (int i = 0; i < 3; i++) {
                int y = rand() % SCREEN_H;
                int x = rand() % SCREEN_W;
                if (game.screen[y][x] == ' ') {
                    game.screen[y][x] = '.';
                }
            }
        }
    }
    
    // Game over screen
    if (game.game_over) {
        system("clear");
        if (game.victory) {
            printf(COL_BRIGHT COL_GREEN);
            printf("\n\n    ██╗   ██╗██╗ ██████╗████████╗ ██████╗ ██████╗ ██╗   ██╗\n");
            printf("    ██║   ██║██║██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗╚██╗ ██╔╝\n");
            printf("    ██║   ██║██║██║        ██║   ██║   ██║██████╔╝ ╚████╔╝ \n");
            printf("    ╚██╗ ██╔╝██║██║        ██║   ██║   ██║██╔══██╗  ╚██╔╝  \n");
            printf("     ╚████╔╝ ██║╚██████╗   ██║   ╚██████╔╝██║  ██║   ██║   \n");
            printf("      ╚═══╝  ╚═╝ ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   \n");
        } else {
            printf(COL_BRIGHT COL_RED);
            printf("\n\n     ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ \n");
            printf("    ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗\n");
            printf("    ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝\n");
            printf("    ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗\n");
            printf("    ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║\n");
            printf("     ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝\n");
        }
        printf(COL_RESET);
        
        printf("\n\nFinal Stats:\n");
        printf("- Health: %d\n", game.health);
        printf("- Ammo: %d\n", game.ammo);
        printf("- Level: %d\n", game.level);
        printf("- Memory Config: %s\n", MEMORY_SIZE_STR);
        printf("\nThanks for playing RAD-DOOM Enhanced!\n");
    }
    
    return 0;
}
