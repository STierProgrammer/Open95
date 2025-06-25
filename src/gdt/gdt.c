#include "gdt.h"

void initGDT(struct GDT* gdt, struct GDTR* gdtr) {
    // Null descriptor 
    SET_BASE(gdt->NullDescriptor, 0l);
    SET_LIMIT(gdt->NullDescriptor, 0l);
    SET_FLAGS(gdt->NullDescriptor, 0);
    gdt->NullDescriptor.access_byte = 0;

    // Kernel mode code segment
    SET_BASE(gdt->KernelCodeSegment, 0l);
    SET_LIMIT(gdt->KernelCodeSegment, 0xFFFFF);
    SET_FLAGS(gdt->KernelCodeSegment, 0xA);
    gdt->KernelCodeSegment.access_byte = 0x9A;

    // Kernel mode data segment
    SET_BASE(gdt->KernelDataSegment, 0l);
    SET_LIMIT(gdt->KernelDataSegment, 0xFFFFF);
    SET_FLAGS(gdt->KernelDataSegment, 0xC);
    gdt->KernelDataSegment.access_byte = 0x92;
    
    // User mode code segment
    SET_BASE(gdt->UserCodeSegment, 0l);
    SET_LIMIT(gdt->UserCodeSegment, 0xFFFFF);
    SET_FLAGS(gdt->UserCodeSegment, 0xA);
    gdt->UserCodeSegment.access_byte = 0xFA;

    // User mode data segment
    SET_BASE(gdt->UserDataSegment, 0l);
    SET_LIMIT(gdt->UserDataSegment, 0xFFFFF);
    SET_FLAGS(gdt->UserDataSegment, 0xC);
    gdt->UserDataSegment.access_byte = 0xF2;

    // TODO: Task state segment

    gdtr->limit = sizeof(struct GDT) - 1;
    gdtr->base = (uint64_t)&gdt->NullDescriptor;

    loadGDT(gdtr);
}
