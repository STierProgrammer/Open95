#ifndef OPEN95_IDT_H
#define OPEN95_IDT_H

#include <stdint.h>

typedef struct IDTR {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) IDTR;

typedef struct IDTDescriptor {
    uint16_t lower_offset;
    uint16_t segment_selector;
    uint8_t ist;
    uint8_t type_and_attributes;
    uint16_t mid_offset;
    uint32_t high_offset;
    uint32_t reserved;
} __attribute__((packed)) IDTDescriptor;

typedef struct IDT {
    IDTDescriptor descriptors[256];
} __attribute__((packed)) IDT;

void createDescriptor(uint8_t index, uint8_t type_and_attributes, void(*handler)());
void initIDT(void);

#endif
