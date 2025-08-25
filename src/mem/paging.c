#include "include/paging.h"

extern PageTable* pml4;

void init_pml4(void)
{
    uint64_t cr3 = read_cr3();
    pml4 = (PageTable*)((cr3 & ~0xFFF) + get_hhdm_offset());
}

void set_page_entry(PageEntry *entry)
{
    if (!(*entry & PAGE_PRESENT))
    {
        *entry = (PageEntry){0};
        *entry |= PAGE_PRESENT;
        *entry |= PAGE_READ_WRITE;
        *entry |= PAGE_USER_SUPERVISOR;
        *entry |= palloc() & PAGE_PHYSICAL_ADDRESS_MASK;
        memset((void *)((*entry & PAGE_PHYSICAL_ADDRESS_MASK) + get_hhdm_offset()), 0, PAGE_SIZE);
    }
}

void map_page_table(uint64_t physical_addr, uint64_t virtual_addr, uint8_t flags)
{
    uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
    uint64_t pml3_index = (virtual_addr >> 30) & 0x1FF;
    uint64_t pml2_index = (virtual_addr >> 21) & 0x1FF;
    uint64_t pml_index = (virtual_addr >> 12) & 0x1FF;

    set_page_entry(&pml4->entries[pml4_index]);
    
    PageTable *pml3 = (PageTable *)((pml4->entries[pml4_index] & PAGE_PHYSICAL_ADDRESS_MASK) + get_hhdm_offset());
    set_page_entry(&pml3->entries[pml3_index]);
    
    PageTable *pml2 = (PageTable *)((pml3->entries[pml3_index] & PAGE_PHYSICAL_ADDRESS_MASK) + get_hhdm_offset());
    set_page_entry(&pml2->entries[pml2_index]);

    PageTable *pml = (PageTable *)((pml2->entries[pml2_index] & PAGE_PHYSICAL_ADDRESS_MASK) + get_hhdm_offset());
    pml->entries[pml_index] = (physical_addr & PAGE_PHYSICAL_ADDRESS_MASK) | (flags & 0xFFF);
}
