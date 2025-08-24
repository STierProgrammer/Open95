#ifndef OPEN95_MEM_VMM
#define OPEN95_MEM_VMM

#include <stdint.h>

struct VirtualMemoryRegion 
{
    uint64_t base;
    uint64_t size;
    struct VirtualMemoryRegion* next;
    uint8_t flags;
};

#endif