/*
 * Memory Manager for SIM Card Environment
 * Manages the extremely limited memory available on SIM cards
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Memory pools
#define HEAP_SIZE 4096  // 4KB heap (half of our 8KB RAM)

static uint8_t heap[HEAP_SIZE];
static uint16_t heap_ptr = 0;

// Simple bump allocator (no free - SIM card style)
void* sim_malloc(uint16_t size) {
    // Align to 2 bytes
    if (size & 1) size++;
    
    if (heap_ptr + size > HEAP_SIZE) {
        return NULL;  // Out of memory
    }
    
    void* ptr = &heap[heap_ptr];
    heap_ptr += size;
    return ptr;
}

// Reset heap (typically done between APDU commands)
void sim_heap_reset(void) {
    heap_ptr = 0;
    memset(heap, 0, HEAP_SIZE);
}

// Get free memory
uint16_t sim_get_free_memory(void) {
    return HEAP_SIZE - heap_ptr;
}

// Initialize memory manager
void sim_memory_init(void) {
    sim_heap_reset();
}
