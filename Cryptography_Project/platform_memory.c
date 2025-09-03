#include <stdlib.h>
#include <stdint.h>

// Simple heap management for bare metal
static uint8_t heap_memory[8192];  // 8KB heap - adjust as needed
static size_t heap_offset = 0;

void* platform_calloc(size_t n, size_t size) {
    size_t total_size = n * size;

    // Simple bump allocator
    if (heap_offset + total_size > sizeof(heap_memory)) {
        return NULL;  // Out of memory
    }

    void* ptr = &heap_memory[heap_offset];
    heap_offset += total_size;

    // Zero the memory
    memset(ptr, 0, total_size);
    return ptr;
}

void platform_free(void* ptr) {
    // Simple allocator - no actual freeing
    // For production, implement proper heap management
}

// Reset heap (call this to "free" all memory)
void platform_heap_reset(void) {
    heap_offset = 0;
}
