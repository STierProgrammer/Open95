#include "isr_handlers.h"
#include "../serial_ports.h"

void isr_divide_error_handler(void) {
    srputs("Division error!\n\0");
}
