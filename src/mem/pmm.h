#ifndef OPEN95_MEM_PMM_H
#define OPEN95_MEM_PMM_H

#include <stddef.h>
#include <stdint.h>
#include "devices/serial.h"
#include "limine.h"

#define ALIGN_UP(address, alignment) (((address) + (alignment - 1)) & ~((alignment) - 1))
#define ALIGN_DOWN(address, alignment) ((address) & ~((alignment) - 1))

#define PAGE_SIZE 4096

extern struct RegionNode *free_mem_head;
extern uint64_t hhdm_offset;
extern struct limine_memmap_response *memmap;

typedef struct RegionNode
{
    uint64_t base;
    struct RegionNode *next;
} RegionNode;

void pmm_init(void);
uint64_t palloc(void);
void pfree(uint64_t);

#endif
