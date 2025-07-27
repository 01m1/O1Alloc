#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define OMEM_SIZE 1048576  // 1MB

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
    int debug;          // Spare bit
} o_block;

int mem_init() {
    mem = mmap(
        NULL,                           // Use any address in memory
        OMEM_SIZE,                      // Starting size of memory
        PROT_READ | PROT_WRITE,         // Allow read and write to memory
        MAP_PRIVATE | MAP_ANONYMOUS,    // Private, zeroed-out memory, not connected to any file
        -1,                             // Not mapping a file
        0                               // 0 offset into file
    );

    if (mem == MAP_FAILED) {
        perror("mmap failed");
        return -1;
    }
    
    heap_data *o_header = (heap_data *) mem;
    o_header->init_size = OMEM_SIZE;
    o_header->size_left = OMEM_SIZE - sizeof(heap_data);    // Heap metadata struct take sup size

    // Set up the first free block right after the header
    o_block *first_block = (o_block *)((char *)mem + sizeof(heap_data));
    first_block->size = o_header->size_left - sizeof(o_block);
    first_block->next = NULL;
    first_block->free = 1;
    first_block->debug = 0;

    first_block->free = 0;
    o_header->size_left -= first_block->size + sizeof(o_block);

    return 0;
}

void print_heap_state() {
    heap_data *header = (heap_data *)mem;
    o_block *curr = (o_block *)((char *)mem + sizeof(heap_data));

    printf("=== Heap Metadata ===\n");
    printf("Initial size: %zu bytes\n", header->init_size);

    printf("\n=== Blocks ===\n");
    int i = 0;
    while (curr) {
        printf("Block %d at %p: size=%zu, free=%d, next=%p\n",
            i, (void *)curr, curr->size, curr->free, (void *)curr->next);
        curr = curr->next;
        i++;
    }
    printf("==============\n");
    printf("\nSize left:    %zu bytes\n", header->size_left);
}


int o_alloc() {

    return 0;
}

int main() {
    if (mem_init() != 0) {
        return 1;
    }
    
    printf("Memory initialised at: %p\n", mem);
    print_heap_state();

    // Clean up when done
    munmap(mem, OMEM_SIZE);

    return 0;
}
