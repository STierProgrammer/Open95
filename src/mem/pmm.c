#include "pmm.h"

__attribute__((used, section(".limine_requests"))) static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 3};

__attribute__((used, section(".limine_requests"))) static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 3};

struct RegionNode *free_mem_head = NULL;
struct limine_memmap_response *memmap = NULL;
uint64_t hhdm_offset = 0;

void pmm_init(void)
{
    hhdm_offset = hhdm_request.response->offset;
    memmap = memmap_request.response;

    for (size_t i = 0; i < memmap_request.response->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap_request.response->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            uint64_t base = ALIGN_UP(entry->base, PAGE_SIZE);
            uint64_t length = entry->length;
            uint64_t end = ALIGN_DOWN((entry->base + length), PAGE_SIZE);

            for (uint64_t current = base; current < end; current += PAGE_SIZE)
            {
                RegionNode *node = (RegionNode *)(current + hhdm_offset);
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
    RegionNode *node = (RegionNode *)(physc_addr + hhdm_offset);
    node->next = free_mem_head;
    node->base = physc_addr;
    free_mem_head = node;
}
