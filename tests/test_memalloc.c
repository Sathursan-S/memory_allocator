#include "../src/memalloc.h"  // Adjust path if needed
#include <stdio.h>

int main() {
    printf("Starting custom memory allocator tests...\n");

    // Test 1: Initialize and allocate
    printf("Test 1: Allocating memory\n");
    void* ptr1 = custom_malloc(100);
    printf("Allocated 100 bytes at %p\n", ptr1);

    void* ptr2 = custom_malloc(200);
    printf("Allocated 200 bytes at %p\n", ptr2);

    print_free_list();  // Print the current state of the free list

    // Test 2: Free memory
    printf("Test 2: Freeing memory\n");
    custom_free(ptr1);
    printf("Freed 100 bytes from %p\n", ptr1);

    custom_free(ptr2);
    printf("Freed 200 bytes from %p\n", ptr2);

    print_free_list();  // Print the free list after freeing

    // Test 3: Allocate again after free
    printf("Test 3: Reallocating memory\n");
    void* ptr3 = custom_malloc(150);
    printf("Allocated 150 bytes at %p\n", ptr3);

    print_free_list();

    return 0;
}
