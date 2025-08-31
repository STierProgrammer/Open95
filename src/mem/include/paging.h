#ifndef OPEN95_MEM_PAGING_H
#define OPEN95_MEM_PAGING_H

#include <stdint.h>
#include "libc/include/string.h"
#include "mem/include/pmm.h"

#define PAGE_PRESENT (1 << 0)
#define PAGE_READ_WRITE (1 << 1)
#define PAGE_USER_SUPERVISOR (1 << 2)
#define PAGE_WRITE_THROUGH (1 << 3)
#define PAGE_CACHE_DISABLE (1 << 4)
#define PAGE_ACCESS (1 << 5)
#define PAGE_DIRTY (1 << 6)
#define PAGE_PAGE_SIZE (1 << 7)
#define PAGE_GLOBAL (1 << 8)
#define PAGE_ATTRIBUTE_TABLE (1 << 12)

#define PAGE_PHYSICAL_ADDRESS_MASK 0xFFFFFFFFF000

typedef uint64_t PageEntry;

typedef struct PageTable
{
    PageEntry entries[512];
} PageTable;

void map_page_table(uint64_t physical_addr, uint64_t virtual_add, uint16_t flags);
void init_pml4(void);

#endif