/*
 * Text-Based Doom Game Logic
 * Pure gameplay, no sound, no color, just ASCII
 * Designed to fit in 8KB SIM card memory
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Game constants tuned for SIM card memory
#define SCREEN_W 40
#define SCREEN_H 25
#define MAP_W 32
#define MAP_H 32
#define MAX_ENEMIES 8
#define MAX_BULLETS 4
#define BULLET_SPEED 2
#define ENEMY_SPEED 4  // Moves every 4 frames

// Fixed-point math for positions (8.8 format)
#define FP_SHIFT 8
#define FP_SCALE (1 << FP_SHIFT)
#define FP_HALF (FP_SCALE / 2)

// Map tiles
#define TILE_EMPTY   0
#define TILE_WALL    1
#define TILE_EXIT    2
#define TILE_AMMO    3
#define TILE_HEALTH  4

// ASCII characters for display
#define CHAR_EMPTY   ' '
#define CHAR_WALL    '#'
#define CHAR_PLAYER  '@'
#define CHAR_ENEMY   'E'
#define CHAR_BULLET  '*'
#define CHAR_AMMO    'a'
#define CHAR_HEALTH  '+'
#define CHAR_EXIT    'X'
#define CHAR_CORPSE  '%'

// Direction constants
typedef enum {
    DIR_NORTH = 0,
    DIR_EAST = 90,
    DIR_SOUTH = 180,
    DIR_WEST = 270
} Direction;

// Entity structures
typedef struct {
    int16_t x, y;        // Fixed-point position
    int16_t dx, dy;      // Velocity
    bool active;
} Bullet;

typedef struct {
    int16_t x, y;        // Fixed-point position
    uint8_t health;
    uint8_t move_timer;
    bool active;
} Enemy;

// Main game state - must fit in SIM memory!
typedef struct {
    // Player state
    int16_t player_x, player_y;
    uint16_t player_angle;
    uint8_t health;
    uint8_t ammo;
    
    // Game state
    uint8_t level;
    uint16_t frame_count;
    bool game_over;
    bool victory;
    
    // Entities
    Enemy enemies[MAX_ENEMIES];
    Bullet bullets[MAX_BULLETS];
    
    // Map (1 bit per tile would save memory, but we'll use bytes for simplicity)
    uint8_t map[MAP_H][MAP_W];
    
    // Screen buffer
    uint8_t screen[SCREEN_H][SCREEN_W];
} GameState;

// Initialize a level
void init_level(GameState* game, uint8_t level) {
    // Clear map
    memset(game->map, TILE_EMPTY, sizeof(game->map));
    
    // Create walls (border + some interior)
    for (int i = 0; i < MAP_W; i++) {
        game->map[0][i] = TILE_WALL;
        game->map[MAP_H-1][i] = TILE_WALL;
    }
    for (int i = 0; i < MAP_H; i++) {
        game->map[i][0] = TILE_WALL;
        game->map[i][MAP_W-1] = TILE_WALL;
    }
    
    // Add some interior walls based on level
    if (level == 1) {
        // Simple cross pattern
        for (int i = 8; i < 24; i++) {
            game->map[MAP_H/2][i] = TILE_WALL;
            game->map[i][MAP_W/2] = TILE_WALL;
        }
    }
    
    // Place exit
    game->map[MAP_H-2][MAP_W-2] = TILE_EXIT;
    
    // Place some pickups
    game->map[5][5] = TILE_AMMO;
    game->map[MAP_H-5][5] = TILE_HEALTH;
    game->map[5][MAP_W-5] = TILE_AMMO;
    
    // Place player
    game->player_x = 2 * FP_SCALE;
    game->player_y = 2 * FP_SCALE;
    game->player_angle = DIR_EAST;
    
    // Reset entities
    for (int i = 0; i < MAX_ENEMIES; i++) {
        game->enemies[i].active = false;
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        game->bullets[i].active = false;
    }
    
    // Spawn enemies based on level
    int enemy_count = 2 + level;
    if (enemy_count > MAX_ENEMIES) enemy_count = MAX_ENEMIES;
    
    for (int i = 0; i < enemy_count; i++) {
        game->enemies[i].active = true;
        game->enemies[i].health = 2;
        game->enemies[i].move_timer = 0;
        // Place enemies in different quadrants
        switch (i % 4) {
            case 0: game->enemies[i].x = 25 * FP_SCALE; game->enemies[i].y = 5 * FP_SCALE; break;
            case 1: game->enemies[i].x = 5 * FP_SCALE; game->enemies[i].y = 25 * FP_SCALE; break;
            case 2: game->enemies[i].x = 25 * FP_SCALE; game->enemies[i].y = 25 * FP_SCALE; break;
            case 3: game->enemies[i].x = 15 * FP_SCALE; game->enemies[i].y = 15 * FP_SCALE; break;
        }
    }
}

// Check collision with map
bool check_collision(GameState* game, int16_t x, int16_t y) {
    int tile_x = x / FP_SCALE;
    int tile_y = y / FP_SCALE;
    
    if (tile_x < 0 || tile_x >= MAP_W || tile_y < 0 || tile_y >= MAP_H) {
        return true;  // Out of bounds
    }
    
    return game->map[tile_y][tile_x] == TILE_WALL;
}

// Move player
void move_player(GameState* game, int8_t forward, int8_t strafe) {
    if (game->game_over) return;
    
    // Calculate movement vector
    int16_t dx = 0, dy = 0;
    
    if (forward != 0) {
        switch (game->player_angle) {
            case DIR_NORTH: dy = -forward * FP_HALF; break;
            case DIR_EAST:  dx = forward * FP_HALF; break;
            case DIR_SOUTH: dy = forward * FP_HALF; break;
            case DIR_WEST:  dx = -forward * FP_HALF; break;
        }
    }
    
    if (strafe != 0) {
        switch (game->player_angle) {
            case DIR_NORTH: dx = strafe * FP_HALF; break;
            case DIR_EAST:  dy = strafe * FP_HALF; break;
            case DIR_SOUTH: dx = -strafe * FP_HALF; break;
            case DIR_WEST:  dy = -strafe * FP_HALF; break;
        }
    }
    
    // Check collision and move
    int16_t new_x = game->player_x + dx;
    int16_t new_y = game->player_y + dy;
    
    if (!check_collision(game, new_x, new_y)) {
        game->player_x = new_x;
        game->player_y = new_y;
        
        // Check for pickups
        int tile_x = new_x / FP_SCALE;
        int tile_y = new_y / FP_SCALE;
        uint8_t tile = game->map[tile_y][tile_x];
        
        switch (tile) {
            case TILE_AMMO:
                game->ammo += 10;
                if (game->ammo > 99) game->ammo = 99;
                game->map[tile_y][tile_x] = TILE_EMPTY;
                break;
            case TILE_HEALTH:
                game->health += 25;
                if (game->health > 100) game->health = 100;
                game->map[tile_y][tile_x] = TILE_EMPTY;
                break;
            case TILE_EXIT:
                game->victory = true;
                game->game_over = true;
                break;
        }
    }
}

// Turn player
void turn_player(GameState* game, int8_t direction) {
    if (game->game_over) return;
    
    if (direction > 0) {
        game->player_angle += 90;
        if (game->player_angle >= 360) game->player_angle -= 360;
    } else {
        if (game->player_angle == 0) {
            game->player_angle = 270;
        } else {
            game->player_angle -= 90;
        }
    }
}

// Fire bullet
void fire_bullet(GameState* game) {
    if (game->game_over || game->ammo == 0) return;
    
    // Find free bullet slot
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!game->bullets[i].active) {
            game->bullets[i].active = true;
            game->bullets[i].x = game->player_x;
            game->bullets[i].y = game->player_y;
            
            // Set velocity based on player direction
            switch (game->player_angle) {
                case DIR_NORTH:
                    game->bullets[i].dx = 0;
                    game->bullets[i].dy = -BULLET_SPEED * FP_HALF;
                    break;
                case DIR_EAST:
                    game->bullets[i].dx = BULLET_SPEED * FP_HALF;
                    game->bullets[i].dy = 0;
                    break;
                case DIR_SOUTH:
                    game->bullets[i].dx = 0;
                    game->bullets[i].dy = BULLET_SPEED * FP_HALF;
                    break;
                case DIR_WEST:
                    game->bullets[i].dx = -BULLET_SPEED * FP_HALF;
                    game->bullets[i].dy = 0;
                    break;
            }
            
            game->ammo--;
            break;
        }
    }
}

// Update bullets
void update_bullets(GameState* game) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!game->bullets[i].active) continue;
        
        // Move bullet
        game->bullets[i].x += game->bullets[i].dx;
        game->bullets[i].y += game->bullets[i].dy;
        
        // Check wall collision
        if (check_collision(game, game->bullets[i].x, game->bullets[i].y)) {
            game->bullets[i].active = false;
            continue;
        }
        
        // Check enemy collision
        int bx = game->bullets[i].x / FP_SCALE;
        int by = game->bullets[i].y / FP_SCALE;
        
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!game->enemies[j].active) continue;
            
            int ex = game->enemies[j].x / FP_SCALE;
            int ey = game->enemies[j].y / FP_SCALE;
            
            if (bx == ex && by == ey) {
                // Hit!
                game->enemies[j].health--;
                game->bullets[i].active = false;
                
                if (game->enemies[j].health == 0) {
                    game->enemies[j].active = false;
                }
                break;
            }
        }
    }
}

// Simple enemy AI
void update_enemies(GameState* game) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!game->enemies[i].active) continue;
        
        game->enemies[i].move_timer++;
        if (game->enemies[i].move_timer < ENEMY_SPEED) continue;
        game->enemies[i].move_timer = 0;
        
        // Move towards player
        int16_t dx = 0, dy = 0;
        
        if (game->enemies[i].x < game->player_x) dx = FP_HALF;
        else if (game->enemies[i].x > game->player_x) dx = -FP_HALF;
        
        if (game->enemies[i].y < game->player_y) dy = FP_HALF;
        else if (game->enemies[i].y > game->player_y) dy = -FP_HALF;
        
        // Try to move
        int16_t new_x = game->enemies[i].x + dx;
        int16_t new_y = game->enemies[i].y + dy;
        
        if (!check_collision(game, new_x, new_y)) {
            game->enemies[i].x = new_x;
            game->enemies[i].y = new_y;
        }
        
        // Check if enemy reached player
        int ex = game->enemies[i].x / FP_SCALE;
        int ey = game->enemies[i].y / FP_SCALE;
        int px = game->player_x / FP_SCALE;
        int py = game->player_y / FP_SCALE;
        
        if (ex == px && ey == py) {
            // Damage player
            if (game->health > 10) {
                game->health -= 10;
            } else {
                game->health = 0;
                game->game_over = true;
            }
        }
    }
}

// Render game to text screen
void render_game(GameState* game) {
    // Clear screen
    memset(game->screen, CHAR_EMPTY, sizeof(game->screen));
    
    // Draw visible portion of map (centered on player)
    int px = game->player_x / FP_SCALE;
    int py = game->player_y / FP_SCALE;
    
    int view_x = px - SCREEN_W / 2;
    int view_y = py - (SCREEN_H - 3) / 2;  // Leave room for status
    
    for (int sy = 0; sy < SCREEN_H - 2; sy++) {
        for (int sx = 0; sx < SCREEN_W; sx++) {
            int mx = view_x + sx;
            int my = view_y + sy;
            
            if (mx >= 0 && mx < MAP_W && my >= 0 && my < MAP_H) {
                switch (game->map[my][mx]) {
                    case TILE_WALL:   game->screen[sy][sx] = CHAR_WALL; break;
                    case TILE_EXIT:   game->screen[sy][sx] = CHAR_EXIT; break;
                    case TILE_AMMO:   game->screen[sy][sx] = CHAR_AMMO; break;
                    case TILE_HEALTH: game->screen[sy][sx] = CHAR_HEALTH; break;
                    default:          game->screen[sy][sx] = CHAR_EMPTY; break;
                }
            } else {
                game->screen[sy][sx] = CHAR_EMPTY;  // Out of bounds
            }
        }
    }
    
    // Draw entities
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!game->enemies[i].active) continue;
        
        int ex = game->enemies[i].x / FP_SCALE - view_x;
        int ey = game->enemies[i].y / FP_SCALE - view_y;
        
        if (ex >= 0 && ex < SCREEN_W && ey >= 0 && ey < SCREEN_H - 2) {
            game->screen[ey][ex] = CHAR_ENEMY;
        }
    }
    
    // Draw bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!game->bullets[i].active) continue;
        
        int bx = game->bullets[i].x / FP_SCALE - view_x;
        int by = game->bullets[i].y / FP_SCALE - view_y;
        
        if (bx >= 0 && bx < SCREEN_W && by >= 0 && by < SCREEN_H - 2) {
            game->screen[by][bx] = CHAR_BULLET;
        }
    }
    
    // Draw player (always in center)
    game->screen[(SCREEN_H - 3) / 2][SCREEN_W / 2] = CHAR_PLAYER;
    
    // Draw direction indicator
    int dir_x = SCREEN_W / 2;
    int dir_y = (SCREEN_H - 3) / 2;
    switch (game->player_angle) {
        case DIR_NORTH: if (dir_y > 0) game->screen[dir_y - 1][dir_x] = '^'; break;
        case DIR_EAST:  if (dir_x < SCREEN_W - 1) game->screen[dir_y][dir_x + 1] = '>'; break;
        case DIR_SOUTH: if (dir_y < SCREEN_H - 3) game->screen[dir_y + 1][dir_x] = 'v'; break;
        case DIR_WEST:  if (dir_x > 0) game->screen[dir_y][dir_x - 1] = '<'; break;
    }
    
    // Status line (manual formatting for SIM compatibility)
    const char* hp_label = "HP:";
    const char* am_label = " AM:";
    const char* lv_label = " L:";
    int pos = 0;
    
    // HP
    for (int i = 0; hp_label[i]; i++) game->screen[SCREEN_H - 2][pos++] = hp_label[i];
    game->screen[SCREEN_H - 2][pos++] = '0' + (game->health / 100);
    game->screen[SCREEN_H - 2][pos++] = '0' + ((game->health / 10) % 10);
    game->screen[SCREEN_H - 2][pos++] = '0' + (game->health % 10);
    
    // AM  
    for (int i = 0; am_label[i]; i++) game->screen[SCREEN_H - 2][pos++] = am_label[i];
    game->screen[SCREEN_H - 2][pos++] = '0' + (game->ammo / 10);
    game->screen[SCREEN_H - 2][pos++] = '0' + (game->ammo % 10);
    
    // Level
    for (int i = 0; lv_label[i]; i++) game->screen[SCREEN_H - 2][pos++] = lv_label[i];
    game->screen[SCREEN_H - 2][pos++] = '0' + game->level;
    
    // Message line
    if (game->game_over) {
        const char* msg = game->victory ? "VICTORY! You found the exit!" : "GAME OVER - You died!";
        int len = strlen(msg);
        int start = (SCREEN_W - len) / 2;
        for (int i = 0; i < len && start + i < SCREEN_W; i++) {
            game->screen[SCREEN_H - 1][start + i] = msg[i];
        }
    } else {
        const char* help = "WASD=move QE=turn SPC=fire";
        for (int i = 0; help[i] && i < SCREEN_W; i++) {
            game->screen[SCREEN_H - 1][i] = help[i];
        }
    }
}

// Main game update
void update_game(GameState* game) {
    if (!game->game_over) {
        game->frame_count++;
        update_bullets(game);
        update_enemies(game);
    }
}

// Initialize new game
void init_game(GameState* game) {
    memset(game, 0, sizeof(GameState));
    game->health = 100;
    game->ammo = 20;
    game->level = 1;
    init_level(game, 1);
}

// Input processing
void process_game_input(GameState* game, char input) {
    switch (input) {
        case 'w': case 'W': move_player(game, 1, 0); break;
        case 's': case 'S': move_player(game, -1, 0); break;
        case 'a': case 'A': move_player(game, 0, -1); break;
        case 'd': case 'D': move_player(game, 0, 1); break;
        case 'q': case 'Q': turn_player(game, -1); break;
        case 'e': case 'E': turn_player(game, 1); break;
        case ' ': fire_bullet(game); break;
        case 'r': case 'R': 
            if (game->game_over) {
                init_game(game);
            }
            break;
    }
}
