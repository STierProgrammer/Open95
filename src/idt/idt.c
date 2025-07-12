#include "idt.h"
#include "isr_handlers.h"

static struct IDT idt __attribute__((aligned(4096)));
static struct IDTR idtr __attribute__((aligned(16)));

void createDescriptor(uint8_t index, uint8_t type_and_attributes, void (*handler)()) {
  uint64_t offset = (uint64_t)handler;

  IDTDescriptor descriptor = {
      .lower_offset = offset & 0xFFFF,
      .mid_offset = (offset >> 16) & 0xFFFF,
      .high_offset = (offset >> 32) & 0xFFFFFFFF,
      .segment_selector = 0x08,
      .type_and_attributes = type_and_attributes,
      .ist = 0,
      .reserved = 0,
  };

  idt.descriptors[index] = descriptor;
}

void initIDT(void) {
  createDescriptor(0, 0x8E, isr_divide_error);
  createDescriptor(6, 0x8E, isr_invalid_opcode);
  createDescriptor(0x08, 0x8E, isr_double_fault);

  idtr.size = sizeof(idt) - 1;
  idtr.offset = (uint64_t)idt.descriptors;

  asm volatile(
      "lidt %0\n"
      "sti\n" ::"m"(idtr));
}
