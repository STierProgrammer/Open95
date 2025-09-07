#ifndef OPEN95_MEM_PMM_H
#define OPEN95_MEM_PMM_H

#include <stdint.h>
#include "bootstub.h"

#define ALIGN_UP(address, alignment) (((address) + (alignment - 1)) & ~((alignment) - 1))
#define ALIGN_DOWN(address, alignment) ((address) & ~((alignment) - 1))
#define PAGE_SIZE 4096

struct PhysicalMemoryRegion
{
    uint64_t base;
    struct PhysicalMemoryRegion *next;
};

void pmm_init(void);
uint64_t palloc(void);
void pfree(uint64_t physc_addr);

#endif
