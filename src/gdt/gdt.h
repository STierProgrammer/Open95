#ifndef OPEN95_GDT_H
#define OPEN95_GDT_H

#include <stdint.h>

/// @brief Argument `entry` type is struct SegmentDescriptor 
/// @brief Argument `base` is a 64 bit binary number
#define SET_BASE(entry, base) {\
    entry.lower_base = base & 0xFFFF; \
    entry.middle_base = (base >> 16) & 0xFF; \
    entry.higher_base = (base >> 24) & 0xFF; \
    entry.even_higher_base = (base >> 32); \
}

/// @brief Argument `entry` type is struct SegmentDescriptor
/// @brief Argument `limit` is a 20 bit binary number
#define SET_LIMIT(entry, limit) {\
    entry.lower_limit = limit & 0xFFFF; \
    entry.limit_and_flags = ((limit >> 16) & 0x0F) | (entry.limit_and_flags & 0xF0); \
}

/// @brief Argument `entry` type is struct SegmentDescriptor
/// @brief Argument `flags` is a 4 bit binary number
#define SET_FLAGS(entry, flags) {\
    entry.limit_and_flags = (entry.limit_and_flags & 0xF0) | (flags << 4); \
}

typedef struct {
    uint16_t lower_limit;
    uint16_t lower_base;
    uint8_t middle_base;
    uint8_t access_byte;
    // Limit is the low byte and
    // flags is the high byte
    uint8_t limit_and_flags;
    uint8_t higher_base;
    uint32_t even_higher_base;
    uint32_t _reserved;
} __attribute__((packed)) SegmentDescriptor;

struct GDT {
    SegmentDescriptor descriptors[256];
};

void initGDT(struct GDT* src);
extern void setGDT(uint16_t limit, uint64_t base);
extern void reloadSegments(void);

#endif