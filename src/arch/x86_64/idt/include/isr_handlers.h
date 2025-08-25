#ifndef OPEN95_ISR_HANDLERS_H
#define OPEN95_ISR_HANDLERS_H

#include <stdint.h>
#include "devices/include/serial.h"

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

typedef struct ExceptionFrame
{
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

extern void isr_exception();
void isr_exception_handler(ExceptionFrame *eframe);

#endif
