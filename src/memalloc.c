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
    Block *new_block = (Block *)mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (new_block == MAP_FAILED)
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

