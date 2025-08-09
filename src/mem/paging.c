#include "paging.h"

uint64_t read_cr3(void)
{
    uint64_t val;
    __asm__ volatile(
        "mov %%cr3, %0" : "=r"(val));
    return val;
}

PageTable *init_pml4()
{
    uint64_t cr3 = read_cr3();
    PageTable *pml4 = (PageTable *)((cr3 & ~0xFFF) + hhdm_offset);
    return pml4;
}

void set_page_table_entry(PageEntry *entry)
{
    if (!entry->P)
    {
        *entry = (PageEntry){0};
        entry->PHYSC_ADDR = (palloc() >> 12);
        entry->P = 1;
        entry->RW = 1;
        entry->US = 1;
        memset((void *)((entry->PHYSC_ADDR << 12) + hhdm_offset), 0, PAGE_SIZE);
    }
}

void map_page_table(PageTable *pml4, uint64_t physical_addr, uint64_t virtual_addr, uint8_t flags)
{
    uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
    uint64_t pml3_index = (virtual_addr >> 30) & 0x1FF;
    uint64_t pml2_index = (virtual_addr >> 21) & 0x1FF;
    uint64_t pml_index = (virtual_addr >> 12) & 0x1FF;

    set_page_table_entry(&pml4->entries[pml4_index]);

    PageTable *pml3 = (PageTable *)((pml4->entries[pml4_index].PHYSC_ADDR << 12) + hhdm_offset);
    set_page_table_entry(&pml3->entries[pml3_index]);

    PageTable *pml2 = (PageTable *)((pml3->entries[pml3_index].PHYSC_ADDR << 12) + hhdm_offset);
    set_page_table_entry(&pml2->entries[pml2_index]);

    PageTable *pml = (PageTable *)((pml2->entries[pml2_index].PHYSC_ADDR << 12) + hhdm_offset);
    pml->entries[pml_index].P = (flags >> 0) & 0x1;
    pml->entries[pml_index].RW = (flags >> 1) & 0x1;
    pml->entries[pml_index].US = (flags >> 2) & 0x1;
    pml->entries[pml_index].PHYSC_ADDR = physical_addr;
}
