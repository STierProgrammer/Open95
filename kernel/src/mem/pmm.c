#include <stddef.h>
#include "mem/pmm.h"
#include "devices/serial.h"

extern struct KernelParams* kernel_params;

struct PhysicalMemoryRegion *free_mem_head = NULL;

void pmm_init(void)
{
    struct Memmap* memmap = &kernel_params->memmap;

    for (uint64_t i = 0; i < memmap->entry_count; i++)
    {
        if (memmap->entries[i].type == MEMMAP_USABLE)
        {
            uint64_t length = memmap->entries[i].length;
            
            uint64_t base = ALIGN_UP(memmap->entries[i].base, PAGE_SIZE);
            uint64_t end = ALIGN_DOWN((memmap->entries[i].base + length), PAGE_SIZE);

            for (uint64_t current = base; current < end; current += PAGE_SIZE)
            {
                struct PhysicalMemoryRegion *region = (struct PhysicalMemoryRegion*)(current + kernel_params->hhdm);
                region->base = current;
                region->next = free_mem_head;
                free_mem_head = region;
            }
        }
    }

    srprintf("[PMM Initialized]\n");
}

uint64_t palloc(void)
{
    if (!free_mem_head) return (uint64_t)NULL;
    
    struct PhysicalMemoryRegion *region = free_mem_head;
    free_mem_head = free_mem_head->next;
    
    return region->base;
}

void pfree(uint64_t physc_addr)
{
    struct PhysicalMemoryRegion *region = (struct PhysicalMemoryRegion*)(physc_addr + kernel_params->hhdm);
    region->next = free_mem_head;
    region->base = physc_addr;
    free_mem_head = region;
}
