#include "isr_handlers.h"

void isr_exception_handler(ExceptionFrame *eframe)
{
    srprintf("Exception code: %x\n", eframe->exception_code);
    srprintf("Error code: %x\n", eframe->error_code);
    srprintf("RIP: %x\n", eframe->rip);
    for (;;)
        ;
}
