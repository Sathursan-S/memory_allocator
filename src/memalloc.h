#ifndef MEMALLOC_H
#define MEMALLOC_H

#include <stddef.h>  // For size_t

// Block structure to manage memory
typedef struct Block {
    size_t size;         // Size of the memory block
    int free;            // 1 if the block is free, 0 if allocated
    struct Block* next;  // Pointer to the next block in the list
} Block;

// Function prototypes
void* memalloc(size_t size);
void memfree(void* ptr);
void init_pool(size_t size);
void print_free_list();  // Debug function to visualize the free list

#endif // ALLOCATOR_H
