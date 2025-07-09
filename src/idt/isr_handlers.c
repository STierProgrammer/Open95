#include "isr_handlers.h"
#include "../serial_ports.h"

void isr_divide_error_handler(void) {
    write_serial_string("Division error!\n\0");
}
