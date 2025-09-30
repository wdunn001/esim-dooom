/*
 * Minimal Doom Implementation for SIM Cards
 * Based on Doom8088 but drastically reduced for SIM card constraints
 */

#include <stdint.h>
#include <stdbool.h>

// Ultra-minimal Doom constants
#define MAP_WIDTH  32
#define MAP_HEIGHT 32
#define SCREEN_W   40  // Text mode style
#define SCREEN_H   25

// Use common header
#include "sim_doom.h"

// Minimal map (1 bit per cell - wall or not)
static uint8_t map_data[MAP_WIDTH * MAP_HEIGHT / 8];

// Function prototypes
void set_wall(uint8_t x, uint8_t y, bool is_wall);
bool is_wall(uint8_t x, uint8_t y);

// Initialize minimal Doom
void doom_init(GameState* state) {
    state->player_x = 16 << 8;  // Start at (16, 16)
    state->player_y = 16 << 8;
    state->player_angle = 0;
    state->health = 100;
    state->ammo = 50;
    
    // Create a simple box map
    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT / 8; i++) {
        map_data[i] = 0;
    }
    
    // Set walls around edges
    for (int x = 0; x < MAP_WIDTH; x++) {
        set_wall(x, 0, true);
        set_wall(x, MAP_HEIGHT - 1, true);
    }
    for (int y = 0; y < MAP_HEIGHT; y++) {
        set_wall(0, y, true);
        set_wall(MAP_WIDTH - 1, y, true);
    }
}

// Set/get wall in map
void set_wall(uint8_t x, uint8_t y, bool is_wall) {
    if (x >= MAP_WIDTH || y >= MAP_HEIGHT) return;
    
    uint16_t bit_index = y * MAP_WIDTH + x;
    uint16_t byte_index = bit_index / 8;
    uint8_t bit_mask = 1 << (bit_index & 7);
    
    if (is_wall) {
        map_data[byte_index] |= bit_mask;
    } else {
        map_data[byte_index] &= ~bit_mask;
    }
}

bool is_wall(uint8_t x, uint8_t y) {
    if (x >= MAP_WIDTH || y >= MAP_HEIGHT) return true;
    
    uint16_t bit_index = y * MAP_WIDTH + x;
    uint16_t byte_index = bit_index / 8;
    uint8_t bit_mask = 1 << (bit_index & 7);
    
    return (map_data[byte_index] & bit_mask) != 0;
}

// Ultra-simple movement
void doom_move_player(GameState* state, int8_t forward, int8_t strafe, int8_t turn) {
    // Turn
    state->player_angle = (state->player_angle + turn * 10) % 360;
    
    // TODO: Implement strafing
    (void)strafe;  // Suppress warning for now
    
    // Move (simplified - no collision yet)
    if (forward != 0) {
        // Simple trig table would go here
        // For now, just axis-aligned movement
        if (state->player_angle < 45 || state->player_angle > 315) {
            state->player_x += forward << 6;  // Right
        } else if (state->player_angle < 135) {
            state->player_y += forward << 6;  // Down
        } else if (state->player_angle < 225) {
            state->player_x -= forward << 6;  // Left
        } else {
            state->player_y -= forward << 6;  // Up
        }
    }
}

// Render a text-mode "3D" view
void doom_render_view(GameState* state, uint8_t* screen) {
    // Clear screen
    for (int i = 0; i < SCREEN_W * SCREEN_H; i++) {
        screen[i] = ' ';
    }
    
    // Draw top-down view for now (no raycasting yet)
    int px = state->player_x >> 8;
    int py = state->player_y >> 8;
    
    // Draw visible area around player
    for (int dy = -10; dy < 10; dy++) {
        for (int dx = -20; dx < 20; dx++) {
            int mx = px + dx;
            int my = py + dy;
            int sx = SCREEN_W / 2 + dx;
            int sy = SCREEN_H / 2 + dy;
            
            if (sx >= 0 && sx < SCREEN_W && sy >= 0 && sy < SCREEN_H) {
                if (mx == px && my == py) {
                    screen[sy * SCREEN_W + sx] = '@';  // Player
                } else if (is_wall(mx, my)) {
                    screen[sy * SCREEN_W + sx] = '#';  // Wall
                } else {
                    screen[sy * SCREEN_W + sx] = '.';  // Floor
                }
            }
        }
    }
    
    // Status line
    char* status = "HP:100 AM:050";
    for (int i = 0; status[i]; i++) {
        screen[i] = status[i];
    }
}
