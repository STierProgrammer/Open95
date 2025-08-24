#include "limine.h"
#include "misc.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "gdt/gdt.h"
#include "idt/idt.h"
#include "mem/pmm.h"
#include "devices/serial.h"
#include "mem/paging.h"
#include "mem/kheap.h"

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request kernel_address_request = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.
__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

extern char section_text_begin[];
extern char section_text_end[];

extern char section_const_data_begin[];
extern char section_const_data_end[];

extern char section_mut_data_begin[];
extern char section_mut_data_end[];

extern struct limine_memmap_response *memmap;

void map_section(PageTable* pml4, char section_begin[], char section_end[], uint8_t flags) 
{
    uint64_t offset = ALIGN_DOWN((uint64_t)section_end - (uint64_t)section_begin, PAGE_SIZE);
    uint64_t pages = ALIGN_UP(((uint64_t)section_end - (uint64_t)ALIGN_DOWN((uint64_t)section_begin, PAGE_SIZE)), PAGE_SIZE)/PAGE_SIZE;

    for (uint64_t i = 0; i < pages; i++) {    
        map_page_table(pml4, kernel_address_request.response->physical_base + offset + i * PAGE_SIZE, kernel_address_request.response->virtual_base + offset + i * PAGE_SIZE, flags);
    }
}

void map_memmap(PageTable* pml4)
{
    for (uint64_t i = 0; i < memmap->entry_count; i++) 
    {
        struct limine_memmap_entry *entry = memmap->entries[i];
        // TODO: LIMINE_MEMMAP_EXECUTABLE_AND_MODULES
        if (entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE || entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_FRAMEBUFFER) {
            uint64_t base = entry->base;
            uint64_t length = entry->length;
            uint64_t end = (entry->base + length);

            for (uint64_t current = base; current < end; current += PAGE_SIZE)
            {
                map_page_table(pml4, current, current + hhdm_offset, PAGE_PRESENT | PAGE_READ_WRITE);
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

    PageTable* pml4 = init_pml4();
    map_section(pml4, section_text_begin, section_text_end, PAGE_PRESENT);
    map_section(pml4, section_const_data_begin, section_const_data_end, PAGE_PRESENT);
    map_section(pml4, section_mut_data_begin, section_mut_data_end, PAGE_PRESENT | PAGE_READ_WRITE);
    map_memmap(pml4);
    set_cr3((uint64_t)(((uint64_t)pml4) - hhdm_offset));

    init_kheap(pml4);

    hcf();
}
