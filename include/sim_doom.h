/*
 * eSIM Doom - Common Header
 * Shared definitions for the SIM card Doom implementation
 */

#ifndef SIM_DOOM_H
#define SIM_DOOM_H

#include <stdint.h>
#include <stdbool.h>

// SIM Card constraints
#define SIM_RAM_SIZE        8192    // 8KB total RAM
#define SIM_HEAP_SIZE       4096    // 4KB for dynamic allocation
#define SIM_STACK_SIZE      1024    // 1KB stack
#define SIM_GLOBALS_SIZE    3072    // 3KB for global data

// Screen dimensions (text mode)
#define SCREEN_WIDTH        40
#define SCREEN_HEIGHT       25

// APDU Commands
#define CLA_DOOM            0x80
#define INS_INIT_GAME       0x01
#define INS_RENDER_FRAME    0x02
#define INS_PROCESS_INPUT   0x03
#define INS_GET_SCREEN      0x04
#define INS_RESET_GAME      0x05

// APDU Status words
#define SW_SUCCESS          0x9000
#define SW_WRONG_LENGTH     0x6700
#define SW_WRONG_CLASS      0x6E00
#define SW_WRONG_INS        0x6D00
#define SW_ERROR            0x6F00

// Game state structure
typedef struct {
    uint16_t player_x;      // Fixed point position
    uint16_t player_y;
    uint16_t player_angle;  // 0-359 degrees
    uint8_t  current_level;
    uint8_t  health;
    uint8_t  ammo;
    uint8_t  keys;
    uint32_t frame_count;
} GameState;

// Memory management
void* sim_malloc(uint16_t size);
void sim_heap_reset(void);
uint16_t sim_get_free_memory(void);
void sim_memory_init(void);

// APDU handling
void handle_apdu(const uint8_t* cmd_buffer, uint16_t cmd_len, 
                 uint8_t* resp_buffer, uint16_t* resp_len);

// Doom functions
void doom_init(GameState* state);
void doom_render_view(GameState* state, uint8_t* screen);
void doom_move_player(GameState* state, int8_t forward, int8_t strafe, int8_t turn);

// Map functions
void set_wall(uint8_t x, uint8_t y, bool is_wall);
bool is_wall(uint8_t x, uint8_t y);

// 3D rendering (if memory allows)
void render_3d_text(GameState* state, uint8_t* screen);
uint16_t cast_ray(GameState* state, int16_t angle_offset);

// RAD-Doom inspired rendering
void render_rad_inspired(GameState* state, uint8_t* screen);

#endif // SIM_DOOM_H
