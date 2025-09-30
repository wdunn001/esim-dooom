/*
 * Mini Raycaster for SIM Card
 * Shows how to add 3D-like rendering within memory constraints
 */

#include <stdint.h>
#include "sim_doom.h"

// Fixed-point math (8.8 format)
#define FP_SHIFT 8
#define FP_SCALE (1 << FP_SHIFT)

// Simple trig tables (would be in ROM/EEPROM)
// Just 90 values (0-89 degrees), mirror for full circle
static const uint8_t sin_table[90] = {
    0, 4, 9, 13, 18, 22, 27, 31, 36, 40, 44, 49, 53, 58, 62, 66,
    71, 75, 79, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 122,
    126, 130, 133, 137, 140, 144, 147, 150, 154, 157, 160, 163,
    166, 169, 172, 175, 177, 180, 183, 185, 188, 190, 193, 195,
    197, 200, 202, 204, 206, 208, 210, 212, 213, 215, 217, 218,
    220, 221, 223, 224, 225, 226, 228, 229, 230, 231, 232, 232,
    233, 234, 235, 235, 236, 236, 237, 237, 238, 238, 238, 239
};

// Get sine value (0-255 range)
uint8_t get_sin(uint16_t angle) {
    angle = angle % 360;
    if (angle < 90) return sin_table[angle];
    if (angle < 180) return sin_table[180 - angle];
    if (angle < 270) return 256 - sin_table[angle - 180];
    return 256 - sin_table[360 - angle];
}

// Get cosine value (0-255 range)
uint8_t get_cos(uint16_t angle) {
    return get_sin((angle + 90) % 360);
}

// Cast a single ray and return distance to wall
uint16_t cast_ray(GameState* state, int16_t angle_offset) {
    uint16_t ray_angle = (state->player_angle + angle_offset + 360) % 360;
    
    // Ray position (fixed point)
    int16_t ray_x = state->player_x;
    int16_t ray_y = state->player_y;
    
    // Ray direction (simplified)
    int8_t dx = (get_cos(ray_angle) - 128) / 4;  // -32 to +32 range
    int8_t dy = (get_sin(ray_angle) - 128) / 4;
    
    // Step along ray
    uint16_t distance = 0;
    while (distance < 20 * FP_SCALE) {  // Max view distance
        ray_x += dx;
        ray_y += dy;
        distance += 32;  // Approximate distance
        
        // Check for wall hit (simplified)
        uint8_t map_x = ray_x >> FP_SHIFT;
        uint8_t map_y = ray_y >> FP_SHIFT;
        
        if (is_wall(map_x, map_y)) {
            return distance;
        }
    }
    
    return 20 * FP_SCALE;  // Max distance
}

// Render a 3D view to the screen (text mode)
void render_3d_text(GameState* state, uint8_t* screen) {
    // Clear screen
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        screen[i] = ' ';
    }
    
    // Cast rays across field of view
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calculate ray angle (-20 to +20 degrees from player angle)
        int16_t angle_offset = (x - SCREEN_WIDTH/2);
        
        // Cast ray
        uint16_t distance = cast_ray(state, angle_offset);
        
        // Convert distance to wall height
        int wall_height = (10 * FP_SCALE) / (distance >> 4);
        if (wall_height > SCREEN_HEIGHT - 2) wall_height = SCREEN_HEIGHT - 2;
        
        // Draw vertical line
        int y_start = (SCREEN_HEIGHT - wall_height) / 2;
        int y_end = y_start + wall_height;
        
        for (int y = y_start; y < y_end; y++) {
            if (y >= 0 && y < SCREEN_HEIGHT) {
                // Different characters for different distances
                if (distance < 5 * FP_SCALE) {
                    screen[y * SCREEN_WIDTH + x] = '#';
                } else if (distance < 10 * FP_SCALE) {
                    screen[y * SCREEN_WIDTH + x] = '=';
                } else {
                    screen[y * SCREEN_WIDTH + x] = '-';
                }
            }
        }
    }
    
    // Status line
    const char* status = "3D VIEW - TURN WITH A/D";
    for (int i = 0; status[i] && i < SCREEN_WIDTH; i++) {
        screen[i] = status[i];
    }
}

// Test if this fits in memory
// Estimated usage:
// - Sin table: 90 bytes (could be in EEPROM)
// - Code: ~500 bytes
// - Stack: ~100 bytes during execution
// Total: Well within our limits!
