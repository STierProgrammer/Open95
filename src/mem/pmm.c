#include "include/pmm.h"

struct RegionNode *free_mem_head = NULL;

void pmm_init(void)
{
    for (size_t i = 0; i < get_memmap()->entry_count; i++)
    {
        if (get_memmap_entry(i)->type == LIMINE_MEMMAP_USABLE)
        {
            uint64_t length = get_memmap_entry(i)->length;
            
            uint64_t base = ALIGN_UP(get_memmap_entry(i)->base, PAGE_SIZE);
            uint64_t end = ALIGN_DOWN((get_memmap_entry(i)->base + length), PAGE_SIZE);

            for (uint64_t current = base; current < end; current += PAGE_SIZE)
            {
                RegionNode *node = (RegionNode *)(current + get_hhdm_offset());
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
    RegionNode *node = (RegionNode *)(physc_addr + get_hhdm_offset());
    node->next = free_mem_head;
    node->base = physc_addr;
    free_mem_head = node;
}
