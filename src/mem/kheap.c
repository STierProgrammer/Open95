#include "include/kheap.h"

struct KHeapRegion* kheap = (struct KHeapRegion*)KHEAP_START;

void print_kheap(void)
{
    struct KHeapRegion* curr = kheap;
    do {
        srprintf("[%x] size: %x, is_free: %x, next_addr: %x, prev_addr: %x\n", curr->base, curr->size, curr->is_free, curr->next ? curr->next->base : (uint64_t)NULL, curr->prev ? curr->prev->base : (uint64_t)NULL);
        curr = curr->next;
    } while (curr);

    srprintf("\n");
}

void init_kheap(void)
{
    map_page_table(palloc(), KHEAP_START, PAGE_PRESENT | PAGE_READ_WRITE);

    kheap->base = KHEAP_START;
    kheap->size = PAGE_SIZE;
    kheap->is_free = true;
    kheap->next = (struct KHeapRegion*)NULL;
    kheap->prev = (struct KHeapRegion*)NULL;
}

void* kmalloc(uint64_t size)
{
    if (size == 0) return NULL;

    uint64_t full_size = size + sizeof(struct KHeapRegion);

    struct KHeapRegion* curr = kheap;
    while (curr->next && (!curr->is_free || curr->size < full_size + sizeof(struct KHeapRegion))) {
        curr = curr->next;
    }

    if (curr->size < full_size + sizeof(struct KHeapRegion))
    {
        uint64_t pages_needed = ALIGN_UP(full_size, PAGE_SIZE) / PAGE_SIZE;

        if (!curr->is_free) {
            curr->next = (struct KHeapRegion*)((curr->base & ~0xFFF) + PAGE_SIZE);
            curr->next->prev = curr;
            curr = curr->next;
        }

        for (uint64_t i = 1; i <= pages_needed; i++) {
            uint64_t new_page_base = (curr->base & ~0xFFF) + i * PAGE_SIZE;
            map_page_table(palloc(), new_page_base, PAGE_PRESENT | PAGE_READ_WRITE);
            curr->size += PAGE_SIZE;            
        }

    }
    
    struct KHeapRegion* new_region = (struct KHeapRegion*)(curr->base + full_size);

    new_region->prev = curr;
    new_region->next = curr->next;
    new_region->base = curr->base + full_size;
    new_region->size = curr->size - full_size;
    new_region->is_free = true;

    curr->next = new_region;
    curr->size = full_size;
    curr->is_free = false;

    return (void*)(curr->base + sizeof(struct KHeapRegion));
}

void kfree(void* addr)
{
    struct KHeapRegion* curr = kheap;
    while (curr && (curr->base != addr - sizeof(struct KHeapRegion)))
    {
        curr = curr->next;
    }

    if (!curr) return;

    curr->is_free = true;

    while (curr->next && curr->next->is_free) 
    {
        curr->size += curr->next->size;
        curr->next = curr->next->next;
    }

    while (curr->prev && curr->prev->is_free)
    {
        curr->prev->size += curr->size;
        curr->prev->next = curr->next;
        curr->prev = curr->prev->prev;
    }
}