#ifndef BOOTLAYER_MEMORY_H
#define BOOTLAYER_MEMORY_H

#include <stdint.h>
#include <limine.h>

enum {
    MEMMAP_RESERVED,
    MEMMAP_ACPI_RECLAIMABLE,
    MEMMAP_EXECUTABLE_AND_MODULES,
    MEMMAP_ACPI_NVS,
    MEMMAP_BAD_MEMORY,
    MEMMAP_BOOTLOADER_RECLAIMABLE,
    MEMMAP_FRAMEBUFFER,
    MEMMAP_USABLE,
};

struct MemmapEntry {
    uint64_t base;
    uint64_t length;
    uint64_t type;
};

struct KernelAddress {
    uint64_t physical_base;
    uint64_t virtual_base;
};



extern volatile struct limine_hhdm_request hhdm_request;
extern volatile struct limine_memmap_request memmap_request;
extern volatile struct limine_executable_address_request kernel_address_request;

uint64_t get_hhdm(void);
struct MemmapEntry get_memmap_entry(uint64_t index);
struct KernelAddress get_kernel_address(void);
uint64_t get_memmap_entries_count(void);

#endif