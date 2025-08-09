#ifndef OPEN95_GDT_H
#define OPEN95_GDT_H

#include <stdint.h>

extern void reload_gdt();

typedef struct GDTEntry
{
    uint16_t low_limit;
    uint16_t low_base;
    uint8_t mid_base;
    uint8_t access_byte;
    uint8_t limit_and_flags; ///< Upper 4 bits of limit and 4 bits of flags.
    uint8_t high_base;
} __attribute__((packed)) GDTEntry;

typedef struct GDT
{
    GDTEntry NullDescriptor;
    GDTEntry KernelCodeSegment;
    GDTEntry KernelDataSegment;
    GDTEntry UserCodeSegment;
    GDTEntry UserDataSegment;
} __attribute__((packed)) GDT;

typedef struct GDTR
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDTR;

void initGDT(void);

#endif
