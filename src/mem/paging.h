#ifndef OPEN95_MEM_PAGING_H
#define OPEN95_MEM_PAGING_H

#include <stdint.h>
#include "libc/string.h"
#include "pmm.h"

#define PAGE_PRESENT (1 << 0)
#define PAGE_READ_WRITE (1 << 1)
#define PAGE_USER_SUPERVISOR (1 << 2)
#define PAGE_LEVEL_WRITE_THROUGH (1 << 3)
#define PAGE_LEVEL_CACHE_DISABLE (1 << 4)
#define PAGE_ACCESSED (1 << 5)
#define PAGE_ORDINARY (1 << 10)
#define PAGE_EXECUTE_DISABLE (1 << 63)

#define PAGE_PHYSICAL_ADDRESS_MASK 0xFFFFFFFFF000

extern uint64_t hhdm_offset;

typedef uint64_t PageEntry;

typedef struct PageTable
{
    PageEntry entries[512];
} PageTable;

void map_page_table(PageTable *pml4, uint64_t physical_addr, uint64_t virtual_add, uint8_t flags);
PageTable *init_pml4(void);

#endif