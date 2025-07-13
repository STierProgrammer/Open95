#ifndef OPEN95_MEM_PMM_H
#define OPEN95_MEM_PMM_H

#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096

static struct RegionNode* REGION_NODE_HEAD = NULL;

typedef struct RegionNode {
    uintptr_t base;
    struct RegionNode* next;
    struct RegionNode* prev;
} RegionNode;

void pmm_init();
uintptr_t palloc();

#endif
