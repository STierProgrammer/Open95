#include "paging.h"

uint64_t read_cr3(void)
{
    uint64_t val;
    __asm__ volatile(
        "mov %%cr3, %0" : "=r"(val));
    return val;
}

void set_page_table_entry(PageTable *table, uint64_t index)
{
    if (table->entries[index].P == 0)
    {
        table->entries[index] = (PageEntry){0};
        table->entries[index].PHYSC_ADDR = palloc();
        table->entries[index].P = 1;
        table->entries[index].RW = 1;
        table->entries[index].US = 1;
        memset((void *)(table->entries[index].PHYSC_ADDR + hhdm_offset), 0, PAGE_SIZE);
    }
}

void map_page_table(PageTable *pml4, uint64_t physical_addr, uint64_t virtual_addr, uint8_t flags)
{
    uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
    uint64_t pml3_index = (virtual_addr >> 30) & 0x1FF;
    uint64_t pml2_index = (virtual_addr >> 21) & 0x1FF;
    uint64_t pml_index = (virtual_addr >> 12) & 0x1FF;

    set_page_table_entry(pml4, pml4_index);

    PageTable *pml3 = (PageTable *)(pml4->entries[pml4_index].PHYSC_ADDR + hhdm_offset);
    set_page_table_entry(pml3, pml3_index);

    PageTable *pml2 = (PageTable *)(pml3->entries[pml3_index].PHYSC_ADDR + hhdm_offset);
    set_page_table_entry(pml2, pml2_index);

    PageTable *pml = (PageTable *)(pml2->entries[pml2_index].PHYSC_ADDR + hhdm_offset);
    pml->entries[pml_index].P = 1;
    pml->entries[pml_index].PHYSC_ADDR = physical_addr;
}

PageTable *init_pml4()
{
    uint64_t cr3 = read_cr3();
    PageTable *pml4 = (PageTable *)(cr3 & ~0xFFF);
    return pml4;
}
