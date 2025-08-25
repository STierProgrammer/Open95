#include "limine.h"
#include "include/misc.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "gdt/include/gdt.h"
#include "idt/include/idt.h"
#include "mem/include/pmm.h"
#include "devices/include/serial.h"
#include "mem/include/paging.h"
#include "mem/include/kheap.h"

PageTable* pml4 = NULL;

extern char section_text_begin[];
extern char section_text_end[];

extern char section_const_data_begin[];
extern char section_const_data_end[];

extern char section_mut_data_begin[];
extern char section_mut_data_end[];

void map_section(char section_begin[], char section_end[], uint8_t flags) 
{
    uint64_t offset = ALIGN_DOWN((uint64_t)section_end - (uint64_t)section_begin, PAGE_SIZE);
    uint64_t pages = ALIGN_UP(((uint64_t)section_end - (uint64_t)ALIGN_DOWN((uint64_t)section_begin, PAGE_SIZE)), PAGE_SIZE)/PAGE_SIZE;

    for (uint64_t i = 0; i < pages; i++) {    
        map_page_table(
            get_kernel_address()->physical_base + offset + i * PAGE_SIZE, 
            get_kernel_address()->virtual_base + offset + i * PAGE_SIZE, 
            flags);
    }
}

void map_memmap()
{
    for (uint64_t i = 0; i < get_memmap()->entry_count; i++) 
    {
        struct limine_memmap_entry *entry = get_memmap()->entries[i];
        // TODO: LIMINE_MEMMAP_EXECUTABLE_AND_MODULES
        if (entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE || entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_FRAMEBUFFER) {
            uint64_t base = entry->base;
            uint64_t length = entry->length;
            uint64_t end = (entry->base + length);

            for (uint64_t current = base; current < end; current += PAGE_SIZE)
            {
                map_page_table(current, current + get_hhdm_offset(), PAGE_PRESENT | PAGE_READ_WRITE);
            }
        }
    }
}

void kmain(void)
{
    init_serial();
    init_gdt();
    init_idt();
    pmm_init();

    init_pml4();
    map_section(section_text_begin, section_text_end, PAGE_PRESENT);
    map_section(section_const_data_begin, section_const_data_end, PAGE_PRESENT);
    map_section(section_mut_data_begin, section_mut_data_end, PAGE_PRESENT | PAGE_READ_WRITE);
    map_memmap();
    set_cr3((uint64_t)(((uint64_t)pml4) - get_hhdm_offset()));

    init_kheap(pml4);
    kmalloc(4097);
    print_kheap();

    hcf();
}
