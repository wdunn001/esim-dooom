/*
 * Visual Test - Doom8088 Style
 * Uses similar text mode rendering technique as Doom8088
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

// Screen dimensions (matching our SIM constraints)
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 25

// Doom8088 uses 0xB1 (▒) for dithering
// We'll use different ASCII chars for different "brightness" levels
#define CHAR_EMPTY  ' '
#define CHAR_FAR    '.'    // Far walls
#define CHAR_MID    ':'    // Medium distance  
#define CHAR_NEAR   '='    // Near walls
#define CHAR_SOLID  '#'    // Very close/solid
#define CHAR_PLAYER '@'    // Player

// Simple game state
typedef struct {
    int player_x;
    int player_y;
    int player_angle;  // 0-360 degrees
    uint8_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];
    uint8_t map[32][32];  // Simple map
} Game;

static Game game;

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void init_map() {
    // Clear map
    memset(game.map, 0, sizeof(game.map));
    
    // Create walls (1 = wall, 0 = empty)
    // Border walls
    for (int i = 0; i < 32; i++) {
        game.map[0][i] = 1;
        game.map[31][i] = 1;
        game.map[i][0] = 1;
        game.map[i][31] = 1;
    }
    
    // Some interior walls
    for (int i = 5; i < 15; i++) {
        game.map[10][i] = 1;
        game.map[i][20] = 1;
    }
    
    // A room
    for (int i = 20; i < 28; i++) {
        game.map[20][i] = 1;
        game.map[27][i] = 1;
    }
    for (int i = 20; i < 28; i++) {
        game.map[i][20] = 1;
        game.map[i][27] = 1;
    }
}

void init_game() {
    init_map();
    game.player_x = 16;
    game.player_y = 16;
    game.player_angle = 0;
}

// Simple raycasting (inspired by Doom8088's approach)
void render_3d_view() {
    // Clear screen
    memset(game.screen, CHAR_EMPTY, SCREEN_WIDTH * SCREEN_HEIGHT);
    
    // Cast rays for each column
    for (int col = 0; col < SCREEN_WIDTH; col++) {
        // Calculate ray angle
        int ray_angle = game.player_angle + (col - SCREEN_WIDTH/2);
        if (ray_angle < 0) ray_angle += 360;
        if (ray_angle >= 360) ray_angle -= 360;
        
        // Simple ray marching
        float ray_x = game.player_x;
        float ray_y = game.player_y;
        float dx = 0.1f;  // Step size
        float dy = 0.0f;
        
        // Convert angle to direction (simplified)
        if (ray_angle < 90) {
            dx = 0.1f; dy = -0.1f * (ray_angle / 90.0f);
        } else if (ray_angle < 180) {
            dx = 0.1f * ((180 - ray_angle) / 90.0f); dy = 0.1f;
        } else if (ray_angle < 270) {
            dx = -0.1f; dy = 0.1f * ((270 - ray_angle) / 90.0f);
        } else {
            dx = -0.1f * ((360 - ray_angle) / 90.0f); dy = -0.1f;
        }
        
        // March the ray
        float distance = 0;
        while (distance < 20) {
            ray_x += dx;
            ray_y += dy;
            distance += 0.1f;
            
            // Check for wall hit
            int map_x = (int)ray_x;
            int map_y = (int)ray_y;
            
            if (map_x >= 0 && map_x < 32 && map_y >= 0 && map_y < 32) {
                if (game.map[map_y][map_x]) {
                    // Hit a wall! Draw column
                    int wall_height = (int)(10.0f / distance);
                    if (wall_height > SCREEN_HEIGHT - 2) wall_height = SCREEN_HEIGHT - 2;
                    
                    int y_start = (SCREEN_HEIGHT - wall_height) / 2;
                    int y_end = y_start + wall_height;
                    
                    // Choose character based on distance (like Doom8088's shading)
                    char wall_char;
                    if (distance < 3) wall_char = CHAR_SOLID;
                    else if (distance < 6) wall_char = CHAR_NEAR;
                    else if (distance < 10) wall_char = CHAR_MID;
                    else wall_char = CHAR_FAR;
                    
                    for (int y = y_start; y < y_end && y < SCREEN_HEIGHT; y++) {
                        if (y >= 0) {
                            game.screen[y * SCREEN_WIDTH + col] = wall_char;
                        }
                    }
                    break;
                }
            }
        }
    }
    
    // Status line
    sprintf((char*)&game.screen[0], "Angle:%03d X:%02d Y:%02d", 
            game.player_angle, game.player_x, game.player_y);
}

void display_screen() {
    clear_screen();
    
    printf("eSIM Doom - Doom8088 Style Rendering\n");
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
    
    printf("\nControls: W/S=Forward/Back, A/D=Turn, Q=Quit\n");
    printf("Characters: . = far, : = medium, = = near, # = close\n");
}

void move_player(char input) {
    switch(input) {
        case 'w': case 'W':  // Move forward
            {
                float dx = 0, dy = 0;
                if (game.player_angle < 90) {
                    dx = 1; dy = 0;
                } else if (game.player_angle < 180) {
                    dx = 0; dy = 1;
                } else if (game.player_angle < 270) {
                    dx = -1; dy = 0;
                } else {
                    dx = 0; dy = -1;
                }
                
                int new_x = game.player_x + (int)dx;
                int new_y = game.player_y + (int)dy;
                
                if (new_x > 0 && new_x < 32 && new_y > 0 && new_y < 32 &&
                    !game.map[new_y][new_x]) {
                    game.player_x = new_x;
                    game.player_y = new_y;
                }
            }
            break;
            
        case 's': case 'S':  // Move backward
            {
                float dx = 0, dy = 0;
                if (game.player_angle < 90) {
                    dx = -1; dy = 0;
                } else if (game.player_angle < 180) {
                    dx = 0; dy = -1;
                } else if (game.player_angle < 270) {
                    dx = 1; dy = 0;
                } else {
                    dx = 0; dy = 1;
                }
                
                int new_x = game.player_x + (int)dx;
                int new_y = game.player_y + (int)dy;
                
                if (new_x > 0 && new_x < 32 && new_y > 0 && new_y < 32 &&
                    !game.map[new_y][new_x]) {
                    game.player_x = new_x;
                    game.player_y = new_y;
                }
            }
            break;
            
        case 'a': case 'A':  // Turn left
            game.player_angle -= 15;
            if (game.player_angle < 0) game.player_angle += 360;
            break;
            
        case 'd': case 'D':  // Turn right
            game.player_angle += 15;
            if (game.player_angle >= 360) game.player_angle -= 360;
            break;
    }
}

int main(void) {
    printf("eSIM Doom - Doom8088 Style Demo\n");
    printf("This demonstrates text-mode 3D rendering like Doom8088\n");
    printf("but fitting in SIM card memory constraints (8KB)\n");
    printf("\nPress Enter to start...\n");
    getchar();
    
    init_game();
    
    char input;
    while (1) {
        render_3d_view();
        display_screen();
        
        printf("Command: ");
        input = getchar();
        if (input == '\n') continue;
        
        if (input == 'q' || input == 'Q') break;
        
        move_player(input);
        
        // Clear input buffer
        while (getchar() != '\n');
    }
    
    printf("\nThanks for playing!\n");
    printf("This demo shows we can have Doom-like gameplay in just 8KB!\n");
    
    return 0;
}
