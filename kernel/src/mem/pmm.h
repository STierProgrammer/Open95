#ifndef OPEN95_MEM_PMM_H
#define OPEN95_MEM_PMM_H

#include "bootstub.h"
#include <stdint.h>

#define ALIGN_UP(address, alignment) (((address) + (alignment - 1)) & ~((alignment) - 1))
#define ALIGN_DOWN(address, alignment) ((address) & ~((alignment) - 1))
#define PAGE_SIZE 4096

typedef uint64_t physc_addr_t;

struct PhysicalMemoryRegion
{
    physc_addr_t base;
    struct PhysicalMemoryRegion *next;
};

void pmm_init(void);
physc_addr_t palloc(void);
void pfree(physc_addr_t physc_addr);

#endif
