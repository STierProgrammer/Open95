#include "isr_handlers.h"
#include "../serial_ports.h"

void isr_divide_error_handler(void) {
    srprintf("Division error!\n");
}
