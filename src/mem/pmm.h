#ifndef OPEN95_MEM_PMM_H
#define OPEN95_MEM_PMM_H

#include <stddef.h>
#include <stdint.h>
#include "../serial_ports/serial_ports.h"
#include "../limine/limine.h"

#define ALIGN_UP(address, alignment) (address + alignment - 1) & ~(alignment -1)
#define ALIGN_DOWN(address, alignment) (address & ~(alignment - 1))

#define PAGE_SIZE 4096

extern struct RegionNode* free_mem_head;
extern uint64_t hhdm_offset;

typedef struct RegionNode {
    uintptr_t base;
    struct RegionNode* next;
} RegionNode;

void pmmInit(void);
uintptr_t palloc(void);
void pfree(void);

#endif
