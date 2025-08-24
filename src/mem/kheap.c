#include "kheap.h"

struct KHeapRegion* kheap = (struct KHeapRegion*)KHEAP_START;

void print_kheap(void)
{
    struct KHeapRegion* curr = kheap;
    while (curr)
    {
        srprintf("[%x] size: %x, is_free: %x, next_addr: %x\n", curr->base, curr->size, curr->is_free, curr->next ? curr->next->base : (uint64_t)NULL);
        curr = curr->next;
    }
}

void init_kheap(PageTable* pml4)
{
    map_page_table(pml4, palloc(), KHEAP_START, PAGE_PRESENT | PAGE_READ_WRITE);

    kheap->base = KHEAP_START;
    kheap->size = 4096;
    kheap->is_free = true;
    kheap->next = (struct KHeapRegion*)NULL;
}

void* kmalloc(PageTable* pml4, uint64_t size)
{
    struct KHeapRegion* region = kheap;
    
    while (region) {
        if (region->is_free) break;
        if (region->next) region = region->next;
        else return (void*)NULL;
    }
    
    if (region->size <= size) {
        uint64_t pages = (size - size % 4096) / 4096;
        uint64_t aligned_base = region->base + 4096 - region->base % 4096;

        for (uint64_t page = 0; page < pages; page++) {
            uint64_t addr = aligned_base + 4096 * page;
            map_page_table(pml4, palloc(), addr, PAGE_PRESENT | PAGE_READ_WRITE);
        }

        region->size += 4096 * pages;
    }

    region->is_free = false;

    struct KHeapRegion* new_region = (struct KHeapRegion*)(region->base + size);
    new_region->base = region->base + size;
    new_region->size = region->size - size;
    new_region->is_free = true;
    new_region->next = region->next;

    region->next = new_region;
    region->size = size;

    return (void*)region->base;
}

void kfree(uint64_t addr)
{
    struct KHeapRegion* curr = kheap;
    do {
        if (curr->base == addr)
        {
            curr->is_free = true;

            while (curr->next && curr->next->is_free == true)
            {
                curr->size += curr->next->size;
                curr->next = curr->next->next;
            }

            break;
        } else curr = curr->next;
    } while (curr->next);
}
