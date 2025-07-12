#include "isr_handlers.h"

void isr_divide_error_handler(void) {
    srprintf("Division error!\n");
}

void isr_invalid_opcode_handler(void) {
    srprintf("Invalid opcode!\n");
}

void isr_double_fault_handler(void) {
    srprintf("Double fault!\n");
}
