#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEM_SIZE 1000000  // 1MB

void* mem = NULL;  // Global pointer to memory

typedef struct heap_data {
    size_t init_size;       // Size of heap
    size_t size_left;       // Size left in heap
    int debug;              // Spare bit
} heap_data;

typedef struct block {
    size_t size;        // How many bytes beyond this block have been allocated in the heap
    struct block *next; // Where is the next block in your linked list
    int free;           // Is this memory free, i.e., available to give away?
    int debug;
} o_block;

int mem_init() {
    mem = mmap(
        NULL,                           // Use any address in memory
        MEM_SIZE,                       // Starting size of memory
        PROT_READ | PROT_WRITE,         // Allow read and write to memory
        MAP_PRIVATE | MAP_ANONYMOUS,    // Private, zeroed-out memory, not connected to any file
        -1,                             // Not mapping a file
        0                               // 0 offset into file
    );

    if (mem == MAP_FAILED) {
        perror("mmap failed");
        return -1;
    }

    return 0;
}

int mem_alloc() {

    return 0;
}

int main() {
    if (mem_init() != 0) {
        return 1;
    }

    printf("Memory initialised at: %p\n", mem);

    // Clean up when done
    munmap(mem, MEM_SIZE);

    return 0;
}
