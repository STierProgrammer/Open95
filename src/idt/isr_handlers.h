#ifndef OPEN95_ISR_HANDLERS_H
#define OPEN95_ISR_HANDLERS_H

#include "../serial_ports/serial_ports.h"
#include "idt.h"

extern void isr_divide_error();
extern void isr_debug_exception();
extern void isr_nmi_interrupt();
extern void isr_breakpoint();
extern void isr_overflow();
extern void isr_bound_range_exceeded();
extern void isr_invalid_opcode();
extern void isr_device_not_avaliable();
extern void isr_double_fault();
extern void isr_coprocessor_segment_overrun();
extern void isr_invalid_tss();
extern void isr_segment_not_present();
extern void isr_stack_segment_fault();
extern void isr_general_protection_fault();
extern void isr_page_fault();
extern void isr_floating_point_error();
extern void isr_alignment_check();
extern void isr_machine_check();
extern void isr_simd_floating_point_exception();
extern void isr_virtualization_exception();
extern void isr_control_protection_exception();

extern void isr_exception();
void isr_exception_handler(ExceptionFrame* eframe);

#endif
