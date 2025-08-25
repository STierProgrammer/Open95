#include "include/idt.h"
#include "include/isr_handlers.h"

static struct IDT idt __attribute__((aligned(4096)));
static struct IDTR idtr __attribute__((aligned(16))) = {
    .offset = (uint64_t)idt.descriptors,
    .size = sizeof(idt) - 1,
};

void create_descriptor(uint8_t index, uint8_t type_and_attributes, void (*handler)())
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

void init_idt(void)
{
#define INTERRUPT_GATE 0x8E
  create_descriptor(0x00, INTERRUPT_GATE, isr_divide_error);
  create_descriptor(0x01, INTERRUPT_GATE, isr_debug_exception);
  create_descriptor(0x02, INTERRUPT_GATE, isr_nmi_interrupt);
  create_descriptor(0x03, INTERRUPT_GATE, isr_breakpoint);
  create_descriptor(0x04, INTERRUPT_GATE, isr_overflow);
  create_descriptor(0x05, INTERRUPT_GATE, isr_bound_range_exceeded);
  create_descriptor(0x06, INTERRUPT_GATE, isr_invalid_opcode);
  create_descriptor(0x07, INTERRUPT_GATE, isr_device_not_avaliable);
  create_descriptor(0x08, INTERRUPT_GATE, isr_double_fault);
  create_descriptor(0x09, INTERRUPT_GATE, isr_coprocessor_segment_overrun);
  create_descriptor(0x0A, INTERRUPT_GATE, isr_invalid_tss);
  create_descriptor(0x0B, INTERRUPT_GATE, isr_segment_not_present);
  create_descriptor(0x0C, INTERRUPT_GATE, isr_stack_segment_fault);
  create_descriptor(0x0D, INTERRUPT_GATE, isr_general_protection_fault);
  create_descriptor(0x0E, INTERRUPT_GATE, isr_page_fault);
  create_descriptor(0x10, INTERRUPT_GATE, isr_floating_point_error);
  create_descriptor(0x11, INTERRUPT_GATE, isr_alignment_check);
  create_descriptor(0x12, INTERRUPT_GATE, isr_machine_check);
  create_descriptor(0x13, INTERRUPT_GATE, isr_simd_floating_point_exception);
  create_descriptor(0x14, INTERRUPT_GATE, isr_virtualization_exception);
  create_descriptor(0x15, INTERRUPT_GATE, isr_control_protection_exception);

  __asm__ volatile(
      "lidt %0\n"
      "sti\n"
      :
      : "m"(idtr));

  srprintf("[IDT Initialized]\n");
}
