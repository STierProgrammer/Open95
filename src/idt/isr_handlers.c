#include "isr_handlers.h"

void isr_divide_error_handler(void) {
    while (1) {
        asm volatile("hlt");
    }
}
