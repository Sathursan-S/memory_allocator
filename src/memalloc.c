#include "memalloc.h"
#include <sys/mman.h> // For mmap
#include <unistd.h>   // For getpagesize
#include <stdio.h>    // For perror
#include <stdlib.h>   // For exit

static Block *free_list = NULL; // Global pointer to the free list

void init_pool(size_t size)
{
    // Ensure the requested size includes space for metadata
    size_t total_size = size + sizeof(Block);

    // Allocate memory using mmap
    free_list = (Block *)mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                              MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (free_list == MAP_FAILED)
    {
        perror("mmap failed");
        exit(EXIT_FAILURE); // Exit if mmap fails
    }

    // Initialize the first free block
    free_list->size = size; // Usable size
    free_list->free = 1;    // Mark as free
    free_list->next = NULL; // No next block yet
}
