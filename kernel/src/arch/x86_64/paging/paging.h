#ifndef OPEN95_MEM_PAGING_H
#define OPEN95_MEM_PAGING_H

#include <stdint.h>

extern char section_text_begin[];
extern char section_text_end[];
extern char section_const_data_begin[];
extern char section_const_data_end[];
extern char section_mut_data_begin[];
extern char section_mut_data_end[];

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

typedef uint64_t physc_addr_t;
typedef uint64_t virt_addr_t;

struct PageTable
{
    uint64_t entries[512];
};

void init_pml4(void);
void map_page_table(uint64_t physical_addr, uint64_t virtual_add, uint16_t flags);
void map_kernel(void);
void map_memmap(void);

#endif