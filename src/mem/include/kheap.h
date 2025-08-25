#ifndef OPEN95_MEM_KHEAP
#define OPEN95_MEM_KHEAP

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "mem/include/paging.h"
#include "devices/include/serial.h"

#define KHEAP_START 0xffffffff00000000
#define KHEAP_END 0xffffffff80000000

struct KHeapRegion {
    uint64_t base;
    uint64_t size;
    struct KHeapRegion* next;
    bool is_free;
};

void print_kheap(void);
void init_kheap();
void* kmalloc(uint64_t size);
void kfree(uint64_t addr);

#endif
