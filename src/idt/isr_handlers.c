#include "isr_handlers.h"

void isr_exception_handler(ExceptionFrame* eframe) {
    srprintf("Exception code: %x", eframe->exception_code);
    for (;;);
}
