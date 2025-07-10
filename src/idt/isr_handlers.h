#ifndef OPEN95_ISR_HANDLERS_H
#define OPEN95_ISR_HANDLERS_H

extern void isr_divide_error();
void isr_divide_error_handler(void);

extern void isr_invalid_opcode();
void isr_invalid_opcode_handler(void);

extern void isr_idk();
void isr_idk_handler(void);

#endif
