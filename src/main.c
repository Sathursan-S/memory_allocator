#include "memalloc.h"
#include <stdio.h>

int main() {
    printf("Welcome to the custom memory allocator demo!\n");

    // Allocate some memory blocks
    void* ptr1 = custom_malloc(128);
    printf("Allocated 128 bytes at %p\n", ptr1);

    void* ptr2 = custom_malloc(256);
    printf("Allocated 256 bytes at %p\n", ptr2);

    void* ptr3 = custom_malloc(64);
    printf("Allocated 64 bytes at %p\n", ptr3);

    print_free_list();  // Print the free list after allocations

    // Free one block
    custom_free(ptr2);
    printf("Freed 256 bytes at %p\n", ptr2);
    print_free_list();  // Print the free list after freeing

    // Allocate a new block that might reuse the freed block
    void* ptr4 = custom_malloc(200);
    printf("Allocated 200 bytes at %p\n", ptr4);
    print_free_list();  // Print the free list again

    // Free all blocks
    custom_free(ptr1);
    custom_free(ptr3);
    custom_free(ptr4);
    printf("Freed all blocks\n");
    print_free_list();  // Final state of the free list

    return 0;
}
