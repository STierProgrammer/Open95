#include "paging.h"

uint64_t read_cr3(void)
{
    uint64_t val;
    __asm__ volatile("mov %%cr3, %0" : "=r"(val));
    return val;
}

void set_page_table_entry(PageTable *table, uint64_t index)
{
    if (table->entries[index].P == 0)
    {
        table->entries[index] = (PageEntry){0};
        table->entries[index].PHYSC_ADDR = palloc();
        table->entries[index].P = 1;
        memset((void *)(table->entries[index].PHYSC_ADDR + hhdm_offset), 0, PAGE_SIZE);
    }
}

void map(PageTable *PML4, uintptr_t physical_addr, uintptr_t virtual_addr)
{
    uint64_t PML4_INDEX = (virtual_addr >> 39) & 0x1FF;
    uint64_t PML3_INDEX = (virtual_addr >> 30) & 0x1FF;
    uint64_t PML2_INDEX = (virtual_addr >> 21) & 0x1FF;
    uint64_t PML_INDEX = (virtual_addr >> 12) & 0x1FF;

    set_page_table_entry(PML4, PML4_INDEX);

    PageTable *PML3 = (PageTable *)(PML4->entries[PML4_INDEX].PHYSC_ADDR + hhdm_offset);
    set_page_table_entry(PML3, PML3_INDEX);

    PageTable *PML2 = (PageTable *)(PML3->entries[PML3_INDEX].PHYSC_ADDR + hhdm_offset);
    set_page_table_entry(PML2, PML2_INDEX);

    PageTable *PML = (PageTable *)(PML2->entries[PML2_INDEX].PHYSC_ADDR + hhdm_offset);
    set_page_table_entry(PML, PML_INDEX);

    PML->entries[PML_INDEX].PHYSC_ADDR = physical_addr;
}

PageTable *init_pml4()
{
    uintptr_t cr3 = (uintptr_t)readCR3();
    PageTable *pml4 = (PageTable *)((cr3 >> 12) << 12);

    return pml4;
}
