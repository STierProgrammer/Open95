#include "bootlayer/include/bootlayer.h"

#ifdef LIMINE_BOOTLOADER
#include <limine.h>

__attribute__((used, section(".limine_requests")))
volatile LIMINE_BASE_REVISION(3);
__attribute__((used, section(".limine_requests_start")))
volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end")))
volatile LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".limine_requests"))) 
volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 3};

__attribute__((used, section(".limine_requests")))
volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 3};

__attribute__((used, section(".limine_requests"))) 
volatile struct limine_executable_address_request kernel_address_request = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = 3
};

__attribute__((used, section(".limine_requests")))
volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 3
};

uint64_t get_hhdm(void) {
    return hhdm_request.response->offset; 
}

static int from_limine_type(int type) 
{
    switch (type) 
    {
        case LIMINE_MEMMAP_USABLE: return MEMMAP_USABLE;
        case LIMINE_MEMMAP_RESERVED: return MEMMAP_RESERVED;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE: return MEMMAP_ACPI_RECLAIMABLE;
        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES: return MEMMAP_EXECUTABLE_AND_MODULES;
        case LIMINE_MEMMAP_ACPI_NVS: return MEMMAP_ACPI_NVS;
        case LIMINE_MEMMAP_BAD_MEMORY: return MEMMAP_BAD_MEMORY;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE: return MEMMAP_BOOTLOADER_RECLAIMABLE;
        case LIMINE_MEMMAP_FRAMEBUFFER: return MEMMAP_FRAMEBUFFER;
        default: return MEMMAP_UNKNOWN;
    }
}

struct MemmapEntry get_memmap_entry(uint64_t index) 
{
    struct MemmapEntry memmap_entry = {0};
    struct limine_memmap_entry* limine_memmap_entry = memmap_request.response->entries[index];

    memmap_entry.base = limine_memmap_entry->base; 
    memmap_entry.length = limine_memmap_entry->length;
    memmap_entry.type = from_limine_type(limine_memmap_entry->type);

    return memmap_entry;
}

struct KernelAddress get_kernel_address(void) 
{
    struct KernelAddress kaddr = {0};
    kaddr.physical_base = kernel_address_request.response->physical_base;
    kaddr.virtual_base = kernel_address_request.response->virtual_base;

    return kaddr;
}

uint64_t get_memmap_entries_count(void) 
{
    return memmap_request.response->entry_count;
}

#endif
