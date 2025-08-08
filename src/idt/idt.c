#include "idt.h"
#include "isr_handlers.h"

static struct IDT idt __attribute__((aligned(4096)));
static struct IDTR idtr __attribute__((aligned(16)));

void createDescriptor(uint8_t index, uint8_t type_and_attributes, void (*handler)())
{
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

void initIDT(void)
{
#define INTERRUPT_GATE 0x8E
  createDescriptor(0x00, INTERRUPT_GATE, isr_divide_error);
  createDescriptor(0x01, INTERRUPT_GATE, isr_debug_exception);
  createDescriptor(0x02, INTERRUPT_GATE, isr_nmi_interrupt);
  createDescriptor(0x03, INTERRUPT_GATE, isr_breakpoint);
  createDescriptor(0x04, INTERRUPT_GATE, isr_overflow);
  createDescriptor(0x05, INTERRUPT_GATE, isr_bound_range_exceeded);
  createDescriptor(0x06, INTERRUPT_GATE, isr_invalid_opcode);
  createDescriptor(0x07, INTERRUPT_GATE, isr_device_not_avaliable);
  createDescriptor(0x08, INTERRUPT_GATE, isr_double_fault);
  createDescriptor(0x09, INTERRUPT_GATE, isr_coprocessor_segment_overrun);
  createDescriptor(0x0A, INTERRUPT_GATE, isr_invalid_tss);
  createDescriptor(0x0B, INTERRUPT_GATE, isr_segment_not_present);
  createDescriptor(0x0C, INTERRUPT_GATE, isr_stack_segment_fault);
  createDescriptor(0x0D, INTERRUPT_GATE, isr_general_protection_fault);
  createDescriptor(0x0E, INTERRUPT_GATE, isr_page_fault);
  createDescriptor(0x10, INTERRUPT_GATE, isr_floating_point_error);
  createDescriptor(0x11, INTERRUPT_GATE, isr_alignment_check);
  createDescriptor(0x12, INTERRUPT_GATE, isr_machine_check);
  createDescriptor(0x13, INTERRUPT_GATE, isr_simd_floating_point_exception);
  createDescriptor(0x14, INTERRUPT_GATE, isr_virtualization_exception);
  createDescriptor(0x15, INTERRUPT_GATE, isr_control_protection_exception);

  idtr.size = sizeof(idt) - 1;
  idtr.offset = (uint64_t)idt.descriptors;

  __asm__ volatile(
      "lidt %0\n"
      "sti\n"
      :
      : "m"(idtr));
}
