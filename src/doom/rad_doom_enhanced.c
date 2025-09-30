/*
 * RAD-Doom Enhanced - Text Mode Doom with RAD-inspired rendering
 * For 32KB+ SIM cards with advanced features
 * Inspired by RAD-Doom's clever display techniques
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// Enhanced configuration for 32KB+ cards
#define SCREEN_W 80      // Double width for better resolution
#define SCREEN_H 40      // Taller display
#define MAP_W 64         // Much larger maps
#define MAP_H 64
#define MAX_ENEMIES 20
#define MAX_BULLETS 30
#define MAX_PICKUPS 10
#define MAX_PARTICLES 50
#define MAX_WEAPONS 4

// Color support using ANSI escape codes
#define COLOR_SUPPORT 1
#define DITHERING_ENABLED 1

// ANSI color codes
#define COL_RESET   "\033[0m"
#define COL_BLACK   "\033[30m"
#define COL_RED     "\033[31m"
#define COL_GREEN   "\033[32m"
#define COL_YELLOW  "\033[33m"
#define COL_BLUE    "\033[34m"
#define COL_MAGENTA "\033[35m"
#define COL_CYAN    "\033[36m"
#define COL_WHITE   "\033[37m"
#define COL_BRIGHT  "\033[1m"
#define COL_DIM     "\033[2m"

// RAD-Doom inspired dithering patterns
const char DITHER_PATTERNS[4][4] = {
    {' ', '.', ':', '+'},  // Light to dark
    {'#', '%', '@', '#'},  // Wall patterns
    {'-', '=', '~', '^'},  // Floor patterns
    {'*', 'o', 'O', '@'}   // Object patterns
};

// Enhanced tile types
#define TILE_EMPTY      0
#define TILE_WALL       1
#define TILE_EXIT       2
#define TILE_AMMO       3
#define TILE_HEALTH     4
#define TILE_DOOR       5
#define TILE_KEY        6
#define TILE_WEAPON     7
#define TILE_SECRET     8

// Weapon types
typedef struct {
    const char* name;
    uint8_t damage;
    uint8_t ammo_use;
    uint8_t fire_rate;
    uint8_t spread;
    char bullet_char;
} Weapon;

// Particle system for effects
typedef struct {
    int16_t x, y;
    int8_t vx, vy;
    uint8_t life;
    uint8_t type;
    char symbol;
} Particle;

// Enhanced enemy types
typedef struct {
    uint16_t x, y;
    uint8_t type;
    uint8_t health;
    uint8_t state;
    uint8_t frame;
    int8_t dx, dy;
    uint8_t ai_mode;
    uint8_t alert_level;
} Enemy;

// Enhanced game state
typedef struct {
    // Player state
    uint16_t player_x, player_y;
    uint8_t player_angle;
    uint8_t health;
    uint8_t armor;
    uint8_t ammo[4];  // Different ammo types
    uint8_t current_weapon;
    uint8_t keys;
    
    // World state
    uint8_t map[MAP_H][MAP_W];
    Enemy enemies[MAX_ENEMIES];
    Particle particles[MAX_PARTICLES];
    
    // Rendering buffers
    uint8_t screen[SCREEN_H][SCREEN_W];
    uint8_t color_buffer[SCREEN_H][SCREEN_W];
    uint8_t depth_buffer[SCREEN_H][SCREEN_W];
    
    // Game state
    uint32_t frame_count;
    uint16_t kills;
    uint16_t secrets_found;
    uint8_t level;
    bool game_over;
    bool victory;
    
    // RAD-inspired features
    uint8_t lighting_mode;
    uint8_t dither_mode;
    bool interlace_frame;
} GameState;

// Weapon definitions
const Weapon weapons[MAX_WEAPONS] = {
    {"Fist",     10, 0, 10, 0, ' '},
    {"Pistol",   20, 1, 5,  1, '.'},
    {"Shotgun",  60, 1, 8,  3, ':'},
    {"Chaingun", 15, 1, 2,  2, '*'}
};

// Initialize enhanced game
void init_rad_game(GameState* game) {
    memset(game, 0, sizeof(GameState));
    
    // Player setup
    game->player_x = 10 * 256;
    game->player_y = 10 * 256;
    game->health = 100;
    game->armor = 0;
    game->ammo[0] = 50;  // Bullets
    game->ammo[1] = 0;   // Shells
    game->ammo[2] = 0;   // Cells
    game->current_weapon = 1;
    
    // Generate enhanced map with rooms and corridors
    generate_rad_map(game);
    
    // Spawn enemies with different types
    for (int i = 0; i < 10; i++) {
        spawn_rad_enemy(game, i % 3);  // 3 enemy types
    }
}

// RAD-inspired map generation
void generate_rad_map(GameState* game) {
    // Create border walls
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (x == 0 || x == MAP_W-1 || y == 0 || y == MAP_H-1) {
                game->map[y][x] = TILE_WALL;
            } else {
                game->map[y][x] = TILE_EMPTY;
            }
        }
    }
    
    // Generate rooms
    for (int i = 0; i < 8; i++) {
        int room_x = 5 + (i % 4) * 15;
        int room_y = 5 + (i / 4) * 15;
        int room_w = 8 + (i % 3) * 2;
        int room_h = 8 + (i % 2) * 2;
        
        create_room(game, room_x, room_y, room_w, room_h);
    }
    
    // Add doors and secrets
    add_doors_and_secrets(game);
    
    // Place exit
    game->map[MAP_H-5][MAP_W-5] = TILE_EXIT;
}

// RAD-style rendering with dithering and depth
void render_rad_view(GameState* game) {
    // Clear buffers
    memset(game->screen, ' ', sizeof(game->screen));
    memset(game->color_buffer, 0, sizeof(game->color_buffer));
    memset(game->depth_buffer, 255, sizeof(game->depth_buffer));
    
    // Cast rays for pseudo-3D view (top half of screen)
    for (int x = 0; x < SCREEN_W; x++) {
        float angle = (game->player_angle - 30 + (x * 60.0 / SCREEN_W)) * 3.14159 / 180.0;
        cast_rad_ray(game, angle, x);
    }
    
    // Apply dithering and interlacing
    if (DITHERING_ENABLED) {
        apply_rad_dithering(game);
    }
    
    // Render particles
    render_particles(game);
    
    // Draw HUD at bottom
    draw_rad_hud(game);
}

// Enhanced ray casting with texture variation
void cast_rad_ray(GameState* game, float angle, int screen_x) {
    float dx = cos(angle);
    float dy = sin(angle);
    float distance = 0;
    
    while (distance < 30) {
        int map_x = (game->player_x / 256) + (int)(dx * distance);
        int map_y = (game->player_y / 256) + (int)(dy * distance);
        
        if (map_x < 0 || map_x >= MAP_W || map_y < 0 || map_y >= MAP_H) break;
        
        uint8_t tile = game->map[map_y][map_x];
        if (tile != TILE_EMPTY) {
            // Calculate wall height based on distance
            int wall_height = (int)(20.0 / (distance + 0.1));
            int wall_top = (SCREEN_H / 2) - wall_height;
            int wall_bottom = (SCREEN_H / 2) + wall_height;
            
            // Apply depth shading and texture
            for (int y = wall_top; y < wall_bottom && y < SCREEN_H - 10; y++) {
                if (y >= 0) {
                    uint8_t depth = (uint8_t)(distance * 8);
                    if (depth < game->depth_buffer[y][screen_x]) {
                        game->depth_buffer[y][screen_x] = depth;
                        
                        // Select character based on distance and texture
                        char wall_char = select_wall_texture(tile, distance, y - wall_top);
                        game->screen[y][screen_x] = wall_char;
                        
                        // Color based on tile type
                        game->color_buffer[y][screen_x] = get_tile_color(tile);
                    }
                }
            }
            break;
        }
        distance += 0.1;
    }
}

// RAD-inspired dithering for color mixing
void apply_rad_dithering(GameState* game) {
    for (int y = 0; y < SCREEN_H - 10; y++) {
        for (int x = 0; x < SCREEN_W; x++) {
            // Interlaced dithering pattern
            if (game->interlace_frame) {
                if ((x + y) % 2 == 0) {
                    // Apply dither pattern
                    int pattern_idx = (game->depth_buffer[y][x] / 64) % 4;
                    if (game->screen[y][x] == '#') {
                        game->screen[y][x] = DITHER_PATTERNS[1][pattern_idx];
                    }
                }
            }
        }
    }
    game->interlace_frame = !game->interlace_frame;
}

// Particle effects system
void update_particles(GameState* game) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle* p = &game->particles[i];
        if (p->life > 0) {
            p->x += p->vx;
            p->y += p->vy;
            p->vy += 1;  // Gravity
            p->life--;
        }
    }
}

// Spawn particle effect
void spawn_particle(GameState* game, int16_t x, int16_t y, uint8_t type) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (game->particles[i].life == 0) {
            Particle* p = &game->particles[i];
            p->x = x;
            p->y = y;
            p->type = type;
            p->life = 20 + (type * 10);
            
            switch (type) {
                case 0:  // Bullet impact
                    p->vx = (rand() % 5) - 2;
                    p->vy = -(rand() % 3);
                    p->symbol = '*';
                    break;
                case 1:  // Explosion
                    p->vx = (rand() % 9) - 4;
                    p->vy = -(rand() % 5);
                    p->symbol = "o0O@"[rand() % 4];
                    break;
                case 2:  // Blood
                    p->vx = (rand() % 3) - 1;
                    p->vy = rand() % 2;
                    p->symbol = '.';
                    break;
            }
            break;
        }
    }
}

// Enhanced HUD with color
void draw_rad_hud(GameState* game) {
    int hud_y = SCREEN_H - 8;
    
    // Clear HUD area
    for (int y = hud_y; y < SCREEN_H; y++) {
        for (int x = 0; x < SCREEN_W; x++) {
            game->screen[y][x] = ' ';
        }
    }
    
    // Draw status bar frame
    for (int x = 0; x < SCREEN_W; x++) {
        game->screen[hud_y][x] = '=';
    }
    
    // Player status
    char status[SCREEN_W];
    snprintf(status, SCREEN_W, 
        " HP:%03d AP:%03d [%s] Ammo:%02d K:%03d S:%d/%d L:%d ",
        game->health, game->armor,
        weapons[game->current_weapon].name,
        game->ammo[game->current_weapon > 0 ? 0 : 0],
        game->kills, game->secrets_found, 5, game->level
    );
    
    for (int i = 0; i < strlen(status) && i < SCREEN_W; i++) {
        game->screen[hud_y + 2][i] = status[i];
    }
    
    // Weapon list
    for (int i = 0; i < MAX_WEAPONS; i++) {
        int x = 5 + i * 15;
        if (x < SCREEN_W - 10) {
            game->screen[hud_y + 4][x] = '1' + i;
            game->screen[hud_y + 4][x + 1] = '.';
            for (int j = 0; j < strlen(weapons[i].name) && x + j + 2 < SCREEN_W; j++) {
                game->screen[hud_y + 4][x + 2 + j] = weapons[i].name[j];
            }
            if (i == game->current_weapon) {
                game->screen[hud_y + 4][x - 1] = '[';
                game->screen[hud_y + 4][x + 10] = ']';
            }
        }
    }
}

// Output with ANSI colors
void display_rad_screen(const GameState* game) {
    printf("\033[H\033[J");  // Clear screen
    
    for (int y = 0; y < SCREEN_H; y++) {
        for (int x = 0; x < SCREEN_W; x++) {
            if (COLOR_SUPPORT && game->color_buffer[y][x] > 0) {
                // Apply color based on buffer
                switch (game->color_buffer[y][x]) {
                    case 1: printf(COL_RED); break;
                    case 2: printf(COL_GREEN); break;
                    case 3: printf(COL_BLUE); break;
                    case 4: printf(COL_YELLOW); break;
                    case 5: printf(COL_CYAN); break;
                    case 6: printf(COL_MAGENTA); break;
                    default: printf(COL_WHITE); break;
                }
            }
            
            printf("%c", game->screen[y][x]);
            
            if (COLOR_SUPPORT) {
                printf(COL_RESET);
            }
        }
        printf("\n");
    }
}

// Helper functions (stubs for now)
void spawn_rad_enemy(GameState* game, uint8_t type) {
    // Implementation here
}

void create_room(GameState* game, int x, int y, int w, int h) {
    // Implementation here
}

void add_doors_and_secrets(GameState* game) {
    // Implementation here
}

void render_particles(GameState* game) {
    // Implementation here
}

char select_wall_texture(uint8_t tile, float distance, int y_offset) {
    // Select texture based on tile type and distance
    if (distance < 5) return '#';
    else if (distance < 10) return '%';
    else if (distance < 15) return '=';
    else return ':';
}

uint8_t get_tile_color(uint8_t tile) {
    switch (tile) {
        case TILE_WALL: return 1;    // Red
        case TILE_EXIT: return 2;    // Green
        case TILE_DOOR: return 4;    // Yellow
        case TILE_SECRET: return 5;  // Cyan
        default: return 7;           // White
    }
}
