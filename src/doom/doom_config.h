/*
 * Text Doom Configuration Header
 * Allows building for different memory targets
 */

#ifndef DOOM_CONFIG_H
#define DOOM_CONFIG_H

// Memory configuration levels
#define MINIMAL   1  // 8KB RAM (basic SIM cards)
#define STANDARD  2  // 32KB RAM (modern SIM cards)
#define ENHANCED  3  // 64KB+ RAM (high-end eSIM)

// Set default if not specified
#ifndef MEMORY_CONFIG
#define MEMORY_CONFIG MINIMAL
#endif

// Configuration parameters based on memory
#if MEMORY_CONFIG == MINIMAL
    #define MAX_ENEMIES 3
    #define MAX_BULLETS 5
    #define MAX_PICKUPS 2
    #define MAP_W 20
    #define MAP_H 20
    #define SCREEN_W 40
    #define SCREEN_H 25
    #define MAX_LEVELS 1
    #define ENABLE_SAVE_STATES 0
    #define ENABLE_MULTIPLE_WEAPONS 0
    #define MEMORY_SIZE_STR "8KB"

#elif MEMORY_CONFIG == STANDARD
    #define MAX_ENEMIES 10
    #define MAX_BULLETS 20
    #define MAX_PICKUPS 8
    #define MAP_W 40
    #define MAP_H 30
    #define SCREEN_W 40
    #define SCREEN_H 25
    #define MAX_LEVELS 5
    #define ENABLE_SAVE_STATES 1
    #define ENABLE_MULTIPLE_WEAPONS 1
    #define MEMORY_SIZE_STR "32KB"

#elif MEMORY_CONFIG == ENHANCED
    #define MAX_ENEMIES 20
    #define MAX_BULLETS 50
    #define MAX_PICKUPS 15
    #define MAP_W 60
    #define MAP_H 40
    #define SCREEN_W 80
    #define SCREEN_H 30
    #define MAX_LEVELS 10
    #define ENABLE_SAVE_STATES 1
    #define ENABLE_MULTIPLE_WEAPONS 1
    #define ENABLE_PARTICLE_EFFECTS 1
    #define ENABLE_ADVANCED_AI 1
    #define MEMORY_SIZE_STR "64KB+"
#endif

// Feature flags
#define HAS_SAVE_STATES (ENABLE_SAVE_STATES == 1)
#define HAS_MULTIPLE_WEAPONS (ENABLE_MULTIPLE_WEAPONS == 1)
#define HAS_PARTICLE_EFFECTS (ENABLE_PARTICLE_EFFECTS == 1)
#define HAS_ADVANCED_AI (ENABLE_ADVANCED_AI == 1)

// Memory usage estimation
#if MEMORY_CONFIG == MINIMAL
    #define ESTIMATED_RAM_USAGE 6144  // ~6KB
#elif MEMORY_CONFIG == STANDARD
    #define ESTIMATED_RAM_USAGE 24576 // ~24KB
#else
    #define ESTIMATED_RAM_USAGE 49152 // ~48KB
#endif

#endif // DOOM_CONFIG_H
