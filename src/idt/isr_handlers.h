#ifndef OPEN95_ISR_HANDLERS_H
#define OPEN95_ISR_HANDLERS_H

#include "../serial_ports/serial_ports.h"
#include "idt.h"

extern void isr_divide_error();
extern void isr_invalid_opcode();
extern void isr_double_fault();

extern void isr_exception();
void isr_exception_handler(ExceptionFrame* eframe);

#endif
