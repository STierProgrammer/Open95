#include "gdt.h"

extern void loadGDT(struct GDTR *gdtr);

static struct GDT gdt = {
    .NullDescriptor = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .highest_base = 0,
        .low_limit = 0,
        .limit_and_flags = 0,
        .access_byte = 0,
        .reserved = 0},
    .KernelCodeSegment = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .highest_base = 0,
        .low_limit = 0x0000,
        .limit_and_flags = 0x0A,
        .access_byte = 0x9A,
    },
    .KernelDataSegment = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .highest_base = 0,
        .low_limit = 0x0000,
        .limit_and_flags = 0x0C,
        .access_byte = 0x92
    },
    .UserCodeSegment = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .highest_base = 0,
        .low_limit = 0x0000,
        .limit_and_flags = 0x0A,
        .access_byte = 0xFA
    },
    .UserDataSegment = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .highest_base = 0,
        .low_limit = 0x0000,
        .limit_and_flags = 0x0C,
        .access_byte = 0xF2
    }
};

static struct GDTR gdtr;

void initGDT(void)
{
    gdtr.limit = sizeof(struct GDT) - 1;
    gdtr.base = (uint64_t)&gdt.NullDescriptor;
 
    loadGDT(&gdtr);
}
