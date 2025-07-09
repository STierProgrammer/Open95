#include "idt.h"
#include "isr_handlers.h"

static struct IDT idt;
static struct IDTR idtr;

void createDescriptor(uint8_t index, uint8_t type_and_attributes, void(*handler)()) {
    uint64_t offset = (uint64_t)handler;

    IDTDescriptor descriptor = {
        .lower_offset = offset & 0xFFFF,
        .mid_offset = (offset >> 16) & 0xFFFF,
        .high_offset = (offset >> 32) & 0xFFFFFFFF,
        .segment_selector = 0x08,
        .ist = 0,
        .reserved = 0,
        .type_and_attributes = type_and_attributes,
    };

    idt.descriptors[index] = descriptor;
}

void initIDT(void) {
    createDescriptor(0, 0x8E, isr_divide_error);

    idtr.size = sizeof(idt) - 1;
    idtr.offset = (uint64_t)&idt.descriptors;

    asm volatile(
        "lidt %0\n"
        "sti\n"
        ::"m"(idtr)
    );
}
