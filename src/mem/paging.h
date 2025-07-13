#ifndef OPEN95_MEM_PAGING_H
#define OPEN95_MEM_PAGING_H

#include <stdint.h>

#define PAGE_SIZE 4096

typedef struct PageEntry {
    uint64_t P: 1;
    uint64_t RW: 1;
    uint64_t US: 1;
    uint64_t PWT: 1;
    uint64_t PCD: 1;
    uint64_t A: 1;
    uint64_t LOW_AVL: 1;
    uint8_t RSVD: 1;
    uint64_t MID_AVL: 3;
    uintptr_t physical_address: 40;
    uint64_t HIGH_AVL: 10;
    uint64_t XD: 1;
} PageEntry;

typedef struct PageTable {
    PageEntry entries[512];
} PageTable;

#endif
