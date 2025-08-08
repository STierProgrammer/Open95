#ifndef OPEN95_MEM_PAGING_H
#define OPEN95_MEM_PAGING_H

#include <stdint.h>
#include "libc/string.h"
#include "pmm.h"

#define PAGE_SIZE 4096

extern uint64_t hhdm_offset;

typedef struct PageEntry {
    uint64_t P : 1;
    uint64_t RW : 1;
    uint64_t US : 1;
    uint64_t PWT : 1;
    uint64_t PCD : 1;
    uint64_t A : 1;
    uint64_t D : 1;
    uint64_t PAT : 1;
    uint64_t G : 1;

    uint64_t LOW_AVL : 3;
    uint64_t PHYSC_ADDR: 36;
    uint64_t RSVD : 4;
    uint64_t HIGH_AVL : 7;
    uint64_t PK : 4;
    uint64_t XD : 1;
} PageEntry;

typedef struct PageTable {
    PageEntry entries[512];
} PageTable;

uint64_t read_cr3(void);
void map(PageTable* PML4, uintptr_t physical_addr, uintptr_t virtual_addr);
PageTable* init_pml4(void);

#endif
