#include <stddef.h>

#include "mem/pmm.h"

#include "devices/serial.h"

extern struct KernelParams* krnl_params;

struct RegionNode *free_mem_head = NULL;

void pmm_init(struct KernelParams* kernel_params)
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
                RegionNode *node = (RegionNode *)(current + kernel_params->hhdm);
                node->base = current;
                node->next = free_mem_head;
                free_mem_head = node;
            }
        }
    }

    srprintf("[PMM Initialized]\n");
}

uint64_t palloc(void)
{
    if (!free_mem_head) {
        srprintf("Pallocation failed due to free_mem_head not being initialized!\n");
        return (uint64_t)NULL;
    }
    
    RegionNode *node = free_mem_head;
    free_mem_head = free_mem_head->next;
    
    srprintf("Pallocated successfully! %x\n", free_mem_head);

    return node->base;
}

void pfree(uint64_t physc_addr)
{
    RegionNode *node = (RegionNode *)(physc_addr + krnl_params->hhdm);
    node->next = free_mem_head;
    node->base = physc_addr;
    free_mem_head = node;
}
