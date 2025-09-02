#ifndef OPEN95_BOOTSTUB_H
#define OPEN95_BOOTSTUB_H

#include <stdint.h>

enum {
    MEMMAP_RESERVED,
    MEMMAP_ACPI_RECLAIMABLE,
    MEMMAP_EXECUTABLE_AND_MODULES,
    MEMMAP_ACPI_NVS,
    MEMMAP_BAD_MEMORY,
    MEMMAP_BOOTLOADER_RECLAIMABLE,
    MEMMAP_FRAMEBUFFER,
    MEMMAP_USABLE,
    MEMMAP_UNKNOWN
};

struct MemmapEntry {
    uint64_t base;
    uint64_t length;
    uint64_t type;
};

struct Memmap {
    struct MemmapEntry* entries;
    uint64_t entry_count;
};

struct KernelAddress {
    uint64_t physical_base;
    uint64_t virtual_base;
};

struct Framebuffer {
    uint64_t addr;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint8_t memory_model;
};

struct KernelParams 
{
    uint64_t hhdm;
    struct KernelAddress kernel_addr;
    struct Memmap memmap;
    struct Framebuffer* framebuffers;
};

#endif