# Custom Memory Allocator

## **Overview**

This project implements a custom memory allocator in C, providing an alternative to standard dynamic memory management functions like `malloc` and `free`. The allocator manages memory efficiently using a free list and interacts directly with the operating system through system calls like `mmap` and `munmap`.

---

## **Architecture**

### **1. Memory Pool**

The allocator manages a **dynamic memory pool**:

- **Lazy Initialization**: The pool is initialized only when the first memory allocation is requested.
- **Dynamic Expansion**: If the current pool cannot satisfy a request, it dynamically grows by requesting more memory from the OS.
- **Optional Shrinking**: When large free blocks are available at the end of the pool, they are released back to the OS using `munmap`.

### **2. Block Metadata**

Each memory block includes metadata to manage allocation and deallocation:

```c
typedef struct Block {
    size_t size;         // Size of the usable memory in the block
    int free;            // 1 if the block is free, 0 if allocated
    struct Block* next;  // Pointer to the next block in the free list
} Block;

```

### **3. Free List**

The allocator uses a **singly linked list** to track free and allocated blocks:

- **First-Fit Strategy**: It allocates the first available block that is large enough.
- **Merging**: Adjacent free blocks are merged to reduce fragmentation.

---

## **Key Concepts**

### **1. Dynamic Memory Management**

- **Heap vs. Stack**: Unlike stack memory, heap memory is allocated dynamically during program execution.
- **Fragmentation**:
   - **Internal**: Unused space within an allocated block.
   - **External**: Gaps between free blocks that prevent large allocations.

### **2. System Calls**

- **`mmap`**: Allocates memory directly from the OS.
- **`munmap`**: Releases memory back to the OS.

### **3. Allocation and Deallocation**

- __Allocation (`custom_malloc`)__:
   - Finds a suitable free block or expands the pool if necessary.
   - Splits large blocks to minimize internal fragmentation.

- __Deallocation (`custom_free`)__:
   - Marks a block as free.
   - Merges adjacent free blocks.
   - Optionally releases memory back to the OS.

---

## **How to Build and Run**

### **Prerequisites**

- **Linux or WSL (Windows Subsystem for Linux)** with `gcc` and `make` installed.
- **PowerShell** on Windows with a supported C compiler (e.g., `MinGW` or `WSL` integration).

### **Steps**

#### **1. Clone the Repository**

```bash
git clone <repository-url>
cd memory_allocator

```

#### **2. Build the Project**

- **On Linux/WSL**:

```bash
make main  # Builds and runs the main demo program

```

- **On PowerShell**:

```powershell
gcc -o build/main src/main.c src/memalloc.c -I./src
./build/main

```

#### **3. Run Tests**

Run the test suite to verify the implementation:

```bash
make test

```

or manually:

```bash
gcc -o build/test_memalloc tests/test_memalloc.c src/memalloc.c -I./src
./build/test_memalloc

```

#### **4. Clean Up**

To remove compiled files:

```bash
make clean

```

---

## **Usage**

### **Function Descriptions**

- __`void* custom_malloc(size_t size)`__:
   Allocates a block of memory of the given size.
- __`void custom_free(void* ptr)`__:
   Frees a previously allocated block of memory.
- __`void init_pool(size_t size)`__:
   Initializes the memory pool lazily.
- __`void print_free_list()`__:
   Debugging function that prints the current state of the free list.

### **Sample Program**

Here’s a basic usage example from `main.c`:

```c
#include "memalloc.h"
#include <stdio.h>

int main() {
    void* ptr1 = custom_malloc(100);
    void* ptr2 = custom_malloc(200);
    print_free_list();

    custom_free(ptr1);
    custom_free(ptr2);
    print_free_list();

    return 0;
}

```

---

## **Future Enhancements**

- **Thread Safety**: Add locks for multi-threaded applications.
- **Best-Fit Strategy**: Improve block selection to reduce fragmentation.
- **Memory Alignment**: Ensure allocated memory aligns with hardware requirements.
- **Defragmentation**: Implement background merging and compaction of free blocks.

---