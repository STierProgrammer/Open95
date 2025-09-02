#ifndef OPEN95_MEM_VMM
#define OPEN95_MEM_VMM

#include <stdint.h>
#include <stddef.h>

struct VirtualMemoryRegion 
{
    uint64_t base;
    uint64_t size;
    struct VirtualMemoryRegion* next;
    struct VirtualMemoryRegion* prev;
    uint8_t flags;
};

void init_vmm(void);

#endif