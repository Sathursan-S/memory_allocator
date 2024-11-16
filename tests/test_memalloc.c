#include "../src/memalloc.h"
#include <stdio.h>

int main() {
    printf("Starting comprehensive memory allocator tests...\n");

    // Test 1: Basic Allocation
    printf("\nTest 1: Basic Allocation\n");
    void* ptr1 = custom_malloc(100);
    void* ptr2 = custom_malloc(200);
    void* ptr3 = custom_malloc(300);
    printf("Allocated blocks:\n");
    printf("ptr1 = %p, ptr2 = %p, ptr3 = %p\n", ptr1, ptr2, ptr3);
    print_free_list();

    // Test 2: Freeing and Merging
    printf("\nTest 2: Freeing and Merging\n");
    custom_free(ptr2);
    printf("Freed ptr2 (200 bytes)\n");
    print_free_list();

    custom_free(ptr1);
    printf("Freed ptr1 (100 bytes)\n");
    print_free_list();

    custom_free(ptr3);
    printf("Freed ptr3 (300 bytes)\n");
    print_free_list();

    // Test 3: Reuse of Freed Blocks
    printf("\nTest 3: Reuse of Freed Blocks\n");
    void* ptr4 = custom_malloc(150);
    printf("Allocated 150 bytes at ptr4 = %p\n", ptr4);
    print_free_list();

    void* ptr5 = custom_malloc(50);
    printf("Allocated 50 bytes at ptr5 = %p\n", ptr5);
    print_free_list();

    // Test 4: Pool Expansion
    printf("\nTest 4: Pool Expansion\n");
    void* ptr6 = custom_malloc(5000);  // Request a large block
    printf("Allocated 5000 bytes at ptr6 = %p\n", ptr6);
    print_free_list();

    // Test 5: Edge Cases
    printf("\nTest 5: Edge Cases\n");

    // Allocate a block that exactly fits a free block
    custom_free(ptr5);
    printf("Freed ptr5 (50 bytes)\n");
    print_free_list();

    void* ptr7 = custom_malloc(50);  // Exact fit
    printf("Allocated 50 bytes at ptr7 = %p (should reuse freed block)\n", ptr7);
    print_free_list();

    // Free all and shrink the pool
    custom_free(ptr4);
    custom_free(ptr6);
    custom_free(ptr7);
    printf("Freed all blocks\n");
    print_free_list();

    return 0;
}
