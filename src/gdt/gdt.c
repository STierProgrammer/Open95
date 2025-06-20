#include "gdt.h"

void initGDT(struct GDT* src) {
    // Null descriptor 
    SET_BASE(src->descriptors[0], 0l);
    SET_LIMIT(src->descriptors[0], 0);
    SET_FLAGS(src->descriptors[0], 0);
    src->descriptors[0].access_byte = 0;

    // Kernel mode code segment
    SET_BASE(src->descriptors[1], 0l);
    SET_LIMIT(src->descriptors[1], 0xFFFFF);
    SET_FLAGS(src->descriptors[1], 0xA);
    src->descriptors[1].access_byte = 0x9A;

    // Kernel mode data segment
    SET_BASE(src->descriptors[2], 0l);
    SET_LIMIT(src->descriptors[2], 0xFFFFF);
    SET_FLAGS(src->descriptors[2], 0xC);
    src->descriptors[2].access_byte = 0x92;
    
    // User mode code segment
    SET_BASE(src->descriptors[3], 0l);
    SET_LIMIT(src->descriptors[3], 0xFFFFF);
    SET_FLAGS(src->descriptors[3], 0xA);
    src->descriptors[3].access_byte = 0xFA;

    // User mode data segment
    SET_BASE(src->descriptors[4], 0l);
    SET_LIMIT(src->descriptors[4], 0xFFFFF);
    SET_FLAGS(src->descriptors[4], 0xC);
    src->descriptors[4].access_byte = 0xF2;

    // TODO: Task state segment

    setGDT(sizeof(struct GDT) - 1, (uint64_t)src->descriptors);
    reloadSegments();
}