#ifndef OPEN95_MEM_KHEAP
#define OPEN95_MEM_KHEAP

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "mem/include/paging.h"
#include "mem/include/pmm.h"
#include "libc/include/string.h"
#include "devices/include/serial.h"

#define KHEAP_START 0xffffffff00000000
#define KHEAP_END 0xffffffff80000000

struct KHeapRegion {
    uint64_t base;
    uint64_t size;
    struct KHeapRegion* next;
    struct KHeapRegion* prev;
    bool is_free;
};

void print_kheap(void);
void init_kheap(void);
void* kmalloc(uint64_t size);
void* kcalloc(uint64_t num, uint64_t size);
void* krealloc(void *ptr, size_t new_size);
void kfree(void* addr);

#endif
