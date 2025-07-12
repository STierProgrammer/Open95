#ifndef OPEN95_ISR_HANDLERS_H
#define OPEN95_ISR_HANDLERS_H

#include "../serial_ports/serial_ports.h"

extern void isr_divide_error();
void isr_divide_error_handler(void);

extern void isr_invalid_opcode();
void isr_invalid_opcode_handler(void);

extern void isr_double_fault();
void isr_double_fault_handler(void);

#endif
