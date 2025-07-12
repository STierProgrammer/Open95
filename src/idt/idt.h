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

typedef struct ExceptionFrame {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t exception_code;
    uint64_t error_code;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) ExceptionFrame;

void createDescriptor(uint8_t index, uint8_t type_and_attributes, void(*handler)());
void initIDT(void);

#endif
