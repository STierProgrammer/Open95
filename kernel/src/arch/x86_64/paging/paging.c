#include <stddef.h>

#include "misc.h"
#include "paging/paging.h"
#include "mem/pmm.h"

#include "libc/string.h"

extern struct PageTable* pml4;
extern struct KernelParams* kernel_params;

void init_pml4(void)
{
    uint64_t cr3 = read_cr3();
    pml4 = (struct PageTable*)((cr3 & ~0xFFF) + kernel_params->hhdm);
}

static void set_page_entry(uint64_t *entry)
{
    if (!(*entry & PAGE_PRESENT))
    {
        *entry = 0;
        *entry |= PAGE_PRESENT;
        *entry |= PAGE_READ_WRITE;
        *entry |= PAGE_USER_SUPERVISOR;
        *entry |= palloc() & PAGE_PHYSICAL_ADDRESS_MASK;
        memset((void *)((*entry & PAGE_PHYSICAL_ADDRESS_MASK) + kernel_params->hhdm), 0, PAGE_SIZE);
    }
}

void map_page_table(uint64_t physical_addr, uint64_t virtual_addr, uint16_t flags)
{
    uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
    uint64_t pml3_index = (virtual_addr >> 30) & 0x1FF;
    uint64_t pml2_index = (virtual_addr >> 21) & 0x1FF;
    uint64_t pml_index = (virtual_addr >> 12) & 0x1FF;

    set_page_entry(&pml4->entries[pml4_index]);
    
    struct PageTable *pml3 = (struct PageTable*)((pml4->entries[pml4_index] & PAGE_PHYSICAL_ADDRESS_MASK) + kernel_params->hhdm);
    set_page_entry(&pml3->entries[pml3_index]);
    
    struct PageTable *pml2 = (struct PageTable*)((pml3->entries[pml3_index] & PAGE_PHYSICAL_ADDRESS_MASK) + kernel_params->hhdm);
    set_page_entry(&pml2->entries[pml2_index]);

    struct PageTable *pml = (struct PageTable*)((pml2->entries[pml2_index] & PAGE_PHYSICAL_ADDRESS_MASK) + kernel_params->hhdm);
    pml->entries[pml_index] = (physical_addr & PAGE_PHYSICAL_ADDRESS_MASK) | (flags & 0xFFF);
}

static void map_section(char section_begin[], char section_end[], uint8_t flags) 
{
    uint64_t offset = ALIGN_DOWN((uint64_t)section_end - (uint64_t)section_begin, PAGE_SIZE);
    uint64_t pages = ALIGN_UP(((uint64_t)section_end - (uint64_t)ALIGN_DOWN((uint64_t)section_begin, PAGE_SIZE)), PAGE_SIZE)/PAGE_SIZE;

    for (uint64_t i = 0; i < pages; i++) {    
        map_page_table(
            kernel_params->kernel_addr.physical_base + offset + i * PAGE_SIZE, 
            kernel_params->kernel_addr.virtual_base + offset + i * PAGE_SIZE, 
            flags);
    }
}

void map_kernel(void)
{
    map_section(section_text_begin, section_text_end, PAGE_PRESENT);
    map_section(section_const_data_begin, section_const_data_end, PAGE_PRESENT);
    map_section(section_mut_data_begin, section_mut_data_end, PAGE_PRESENT | PAGE_READ_WRITE);       
}


void map_memmap(void)
{
    for (uint64_t i = 0; i < kernel_params->memmap.entry_count; i++) 
    {
        struct MemmapEntry entry = kernel_params->memmap.entries[i];
        // TODO: Add MEMMAP_EXECUTABLE_AND_MODULES
        if (entry.type == MEMMAP_BOOTLOADER_RECLAIMABLE || entry.type == MEMMAP_USABLE || entry.type == MEMMAP_FRAMEBUFFER) {
            uint64_t base = entry.base;
            uint64_t length = entry.length;
            uint64_t end = (entry.base + length);

            for (uint64_t current = base; current < end; current += PAGE_SIZE)
            {
                if (entry.type == MEMMAP_FRAMEBUFFER) {
                    map_page_table(current, current + kernel_params->hhdm, PAGE_PRESENT | PAGE_READ_WRITE | PAGE_ATTRIBUTE_TABLE | PAGE_WRITE_THROUGH);
                } else {
                    map_page_table(current, current + kernel_params->hhdm, PAGE_PRESENT | PAGE_READ_WRITE);
                }
            }
        }
    }
}