/*
 * eSIM Doom - Common Header
 * Shared definitions for the Text Doom SIM card implementation
 */

#ifndef SIM_DOOM_H
#define SIM_DOOM_H

#include <stdint.h>
#include <stdbool.h>

// SIM Card constraints
#define SIM_RAM_SIZE        8192    // 8KB total RAM

// Screen dimensions (text mode)
#define SCREEN_WIDTH        40
#define SCREEN_HEIGHT       25

// APDU Commands
#define CLA_DOOM            0x80
#define INS_INIT_GAME       0x01
#define INS_PROCESS_INPUT   0x02
#define INS_UPDATE_GAME     0x03
#define INS_GET_SCREEN      0x04
#define INS_GET_STATUS      0x05
#define INS_RESET_GAME      0x06

// APDU Status words
#define SW_SUCCESS          0x9000
#define SW_WRONG_LENGTH     0x6700
#define SW_WRONG_CLASS      0x6E00
#define SW_WRONG_INS        0x6D00
#define SW_ERROR            0x6F00

// Memory management (for SIM environment)
void* sim_malloc(uint16_t size);
void sim_heap_reset(void);
uint16_t sim_get_free_memory(void);
void sim_memory_init(void);

// APDU handling
void handle_apdu(const uint8_t* cmd_buffer, uint16_t cmd_len, 
                 uint8_t* resp_buffer, uint16_t* resp_len);

#endif // SIM_DOOM_H