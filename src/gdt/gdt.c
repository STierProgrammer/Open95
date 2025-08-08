#include "gdt.h"

static struct GDT gdt = {
    .NullDescriptor = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .low_limit = 0,
        .limit_and_flags = 0,
        .access_byte = 0,
    },
    .KernelCodeSegment = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .low_limit = 0xFFFF,
        .access_byte = 0x98,
        .limit_and_flags = 0x20,
    },
    .KernelDataSegment = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .low_limit = 0xFFFF,
        .access_byte = 0x92,
        .limit_and_flags = 0x20, // 0x0C
    },
    .UserCodeSegment = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .low_limit = 0xFFFF,
        .access_byte = 0xF8,
        .limit_and_flags = 0x20, // 0x0A
    },
    .UserDataSegment = {
        .low_base = 0,
        .mid_base = 0,
        .high_base = 0,
        .low_limit = 0xFFFF,
        .access_byte = 0xF2,
        .limit_and_flags = 0x20, // 0x0C
    }
};

static struct GDTR gdtr = {
    .limit = sizeof(struct GDT) - 1,
    .base = (uint64_t)&gdt
};

void initGDT(void)
{
    asm volatile("lgdt %0\n" ::"m"(gdtr));

    reload_gdt();
}
