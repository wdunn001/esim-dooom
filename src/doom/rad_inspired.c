/*
 * RAD-Doom Inspired Techniques for SIM Cards
 * Taking color reduction and dithering concepts from RAD-Doom
 * but adapted for text mode and 8KB memory
 */

#include <stdint.h>
#include <stdbool.h>
#include "sim_doom.h"

// RAD-Doom uses sophisticated dithering matrices
// We'll use a simple 2x2 Bayer matrix for text mode
static const uint8_t bayer_2x2[2][2] = {
    {0, 2},
    {3, 1}
};

// RAD-Doom maps to 16 C64 colors
// We map to ASCII brightness levels
static const char ascii_palette[] = " .:-=+*#%@";
#define PALETTE_SIZE 10

// RAD-Doom tracks color frequency per 4x8 block
// We'll track brightness per 2x2 text block (simplified)
typedef struct {
    uint8_t brightness[4];  // 2x2 block
    uint8_t dominant;       // Most common brightness
} TextBlock;

// Convert a "brightness" value to ASCII using dithering
char brightness_to_ascii_dithered(uint8_t brightness, uint8_t x, uint8_t y) {
    // Apply 2x2 Bayer dithering (inspired by RAD-Doom's approach)
    uint8_t dither = bayer_2x2[y & 1][x & 1];
    uint16_t dithered = brightness + (dither * 64);  // Scale dither pattern
    
    // Map to ASCII palette
    uint8_t index = (dithered >> 8) * PALETTE_SIZE / 256;
    if (index >= PALETTE_SIZE) index = PALETTE_SIZE - 1;
    
    return ascii_palette[index];
}

// RAD-Doom's concept: Reduce colors per block
// Our version: Ensure text readability per 2x2 area
void enforce_block_constraints(uint8_t* screen, uint16_t width, uint16_t height) {
    // Process 2x2 blocks
    for (uint16_t by = 0; by < height; by += 2) {
        for (uint16_t bx = 0; bx < width; bx += 2) {
            TextBlock block;
            uint16_t total = 0;
            
            // Analyze block
            for (uint8_t dy = 0; dy < 2 && (by + dy) < height; dy++) {
                for (uint8_t dx = 0; dx < 2 && (bx + dx) < width; dx++) {
                    uint16_t idx = (by + dy) * width + (bx + dx);
                    uint8_t brightness = 0;
                    
                    // Find brightness from ASCII character
                    for (uint8_t i = 0; i < PALETTE_SIZE; i++) {
                        if (screen[idx] == ascii_palette[i]) {
                            brightness = i * 255 / (PALETTE_SIZE - 1);
                            break;
                        }
                    }
                    
                    block.brightness[dy * 2 + dx] = brightness;
                    total += brightness;
                }
            }
            
            // Find dominant brightness (like RAD-Doom's background color heuristic)
            block.dominant = total / 4;
            
            // Enforce consistency (simplified version of RAD-Doom's color reduction)
            for (uint8_t dy = 0; dy < 2 && (by + dy) < height; dy++) {
                for (uint8_t dx = 0; dx < 2 && (bx + dx) < width; dx++) {
                    uint16_t idx = (by + dy) * width + (bx + dx);
                    
                    // If this cell is too different from dominant, adjust it
                    uint8_t cell_brightness = block.brightness[dy * 2 + dx];
                    if (abs(cell_brightness - block.dominant) > 128) {
                        // Apply dithering to smooth the transition
                        screen[idx] = brightness_to_ascii_dithered(
                            (cell_brightness + block.dominant) / 2,
                            bx + dx, by + dy
                        );
                    }
                }
            }
        }
    }
}

// Frame alternation for flicker fusion (inspired by RAD-Doom)
static uint8_t frame_counter = 0;

// Render with RAD-Doom inspired techniques
void render_rad_inspired(GameState* state, uint8_t* screen) {
    // Clear screen
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        screen[i] = ' ';
    }
    
    frame_counter++;
    
    // Simple 3D rendering with dithering
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Cast ray (simplified)
        int angle_offset = x - SCREEN_WIDTH / 2;
        uint16_t distance = cast_ray(state, angle_offset);
        
        // Convert distance to brightness (inverse relationship)
        uint8_t brightness = 255;
        if (distance < 20 * FP_SCALE) {
            brightness = 255 - (distance * 255 / (20 * FP_SCALE));
        }
        
        // Calculate wall height
        int wall_height = (10 * FP_SCALE) / (distance >> 4);
        if (wall_height > SCREEN_HEIGHT - 2) wall_height = SCREEN_HEIGHT - 2;
        
        int y_start = (SCREEN_HEIGHT - wall_height) / 2;
        int y_end = y_start + wall_height;
        
        // Draw column with dithering
        for (int y = y_start; y < y_end && y < SCREEN_HEIGHT; y++) {
            if (y >= 0) {
                // Apply temporal dithering (frame alternation like RAD-Doom)
                uint8_t temporal_offset = (frame_counter & 1) ? 32 : 0;
                
                screen[y * SCREEN_WIDTH + x] = brightness_to_ascii_dithered(
                    brightness + temporal_offset, x, y
                );
            }
        }
    }
    
    // Apply block constraints (like RAD-Doom's color enforcement)
    enforce_block_constraints(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Status (no dithering for readability)
    const char* status = "RAD-inspired rendering";
    for (int i = 0; status[i] && i < SCREEN_WIDTH; i++) {
        screen[i] = status[i];
    }
}

// Summary: We've adapted RAD-Doom's concepts:
// 1. Dithering matrices → ASCII brightness dithering
// 2. Color reduction → Brightness consistency per block
// 3. Frame alternation → Temporal dithering (if display allows)
// 4. Block constraints → 2x2 text block coherence
// All fitting in our 8KB limit!
