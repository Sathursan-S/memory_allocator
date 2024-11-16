#include "memalloc.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif // For mmap
#include <unistd.h>   // For getpagesize
#include <stdio.h>    // For perror
#include <stdlib.h>   // For exit

static Block *free_list = NULL; // Global pointer to the free list

void* mmap_helper(size_t size);
int munmap_helper(void* addr, size_t size);

void init_pool(size_t size)
{
    // Ensure the requested size includes space for metadata
    size_t total_size = size + sizeof(Block);

    // Allocate memory using mmap
    free_list = (Block *)mmap_helper(total_size);

    if (free_list == NULL)
    {
        perror("mmap failed");
        exit(EXIT_FAILURE); // Exit if mmap fails
    }

    // Initialize the first free block
    free_list->size = size; // Usable size
    free_list->free = 1;    // Mark as free
    free_list->next = NULL; // No next block yet
}

void *memalloc(size_t size)
{
    if (free_list == NULL)
    {
        // Lazily initialize the pool on the first allocation
        init_pool(size);
    }

    Block *current = free_list;
    Block *prev = NULL;

    while (current)
    {
        if (current->free && current->size >= size)
        {
            // If block is large enough, split it
            if (current->size > size + sizeof(Block))
            {
                Block *new_block = (Block *)((char *)current + sizeof(Block) + size); // Calculate the address of the new block
                new_block->size = current->size - size - sizeof(Block);
                new_block->free = 1;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            // Mark the block as allocated
            current->free = 0;
            return (char *)current + sizeof(Block); // Return usable memory
        }

        prev = current;
        current = current->next;
    }

    // No suitable block found, expand the pool
    size_t total_size = size + sizeof(Block);
    Block *new_block = (Block *)mmap_helper(total_size);
    if (new_block == NULL)
    {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    new_block->size = size;
    new_block->free = 0;
    new_block->next = NULL;

    // Add the new block to the end of the free list
    if (prev)
    {
        prev->next = new_block;
    }
    else
    {
        free_list = new_block;
    }

    return (char *)new_block + sizeof(Block);
}

void memfree(void *ptr)
{
    if (!ptr)
        return;

    // Move back to the block metadata
    Block *current = (Block*)((char *)ptr - sizeof(Block));
    current->free = 1; // Mark the block as free

    // Merge with the next block if it's free
    Block *next = current->next;
    if (next && next->free)
    {
        current->size += sizeof(Block) + next->size;
        current->next = next->next;
    }

    // Shrink the pool if this is the last block (end of datasegment or heap)
    if (current->next == NULL)
    {
        if (munmap_helper(current, current->size + sizeof(Block)) == -1)
        {
            perror("munmap failed");
        }
    }
}

void print_free_list() {
    Block* current = free_list;
    printf("Free List:\n");
    while (current) {
        printf("Block at %p: size=%zu, free=%d, next=%p\n",
               (void*)current, current->size, current->free, (void*)current->next);
        current = current->next;
    }
    printf("\n");
}

// helper functions
void* mmap_helper(size_t size) {
#ifdef _WIN32
    void* addr = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (addr == NULL) {
        perror("VirtualAlloc failed");
        return NULL;
    }
#else
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }
#endif
    return addr;
}

int munmap_helper(void* addr, size_t size) {
#ifdef _WIN32
    if (!VirtualFree(addr, 0, MEM_RELEASE)) {
        perror("VirtualFree failed");
        return -1;
    }
#else
    if (munmap(addr, size) == -1) {
        perror("munmap failed");
        return -1;
    }
#endif
    return 0;
}