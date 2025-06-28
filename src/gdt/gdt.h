#ifndef OPEN95_GDT_H
#define OPEN95_GDT_H

#include <stdint.h>

// /**
//  * @brief Sets the base address fields of a GDT entry.
//  * 
//  * This macro splits the 64-bit base address into the respective
//  * parts of the GDT entry structure.
//  * 
//  * @param entry A GDTEntry structure to modify.
//  * @param base A 64-bit base address to set.
//  */
// #define SET_BASE(entry, base) {\
//     entry.low_base = (base) & 0xFFFF; \
//     entry.mid_base = ((base) >> 16) & 0xFF; \
//     entry.high_base = ((base) >> 24) & 0xFF; \
//     entry.highest_base = ((base) >> 32); \
// }

// /**
//  * @brief Sets the segment limit fields of a GDT entry.
//  * 
//  * The segment limit is a 20-bit value split across two fields:
//  * low_limit and the lower 4 bits of limit_and_flags.
//  * This macro updates those fields accordingly without affecting
//  * the upper 4 bits of limit_and_flags.
//  * 
//  * @param entry A GDTEntry structure to modify.
//  * @param limit A 20-bit segment limit value.
//  */
// #define SET_LIMIT(entry, limit) {\
//     entry.low_limit = (limit) & 0xFFFF; \
//     entry.limit_and_flags = ((limit >> 16) & 0x0F) | (entry.limit_and_flags & 0xF0); \
// }

// /**
//  * @brief Sets the flags field of the segment descriptor.
//  * 
//  * The flags occupy the upper 4 bits of the limit_and_flags byte.
//  * This macro preserves the lower 4 bits
//  * and sets the upper 4 bits to the provided flags value.
//  * 
//  * @param entry A GDTEntry structure to modify.
//  * @param flags A 4-bit flags value.
//  */
// #define SET_FLAGS(entry, flags) {\
//     entry.limit_and_flags = (entry.limit_and_flags & 0x0F) | ((flags & 0x0F) << 4); \
// }

typedef struct GDTEntry {
    uint16_t low_limit;
    uint16_t low_base;
    uint8_t mid_base;
    uint8_t access_byte;
    uint8_t limit_and_flags; ///< Upper 4 bits of limit and 4 bits of flags. 
    uint8_t high_base;
    uint32_t highest_base;
    uint32_t reserved;
} __attribute__((packed)) GDTEntry;

typedef struct GDT {
    GDTEntry NullDescriptor;
    GDTEntry KernelCodeSegment;
    GDTEntry KernelDataSegment;
    GDTEntry UserCodeSegment;
    GDTEntry UserDataSegment;
} __attribute__((packed)) GDT;

typedef struct GDTR {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDTR;


void initGDT(void);

#endif