#include "pic.h"

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
        outb(SLAVE_PIC_COMMAND_PORT, CMD_PIC_EOI);
    outb(MASTER_PIC_COMMAND_PORT, CMD_PIC_EOI);
}

void pic_disable(void)
{
    outb(MASTER_PIC_DATA_PORT, 0xff);
    outb(SLAVE_PIC_DATA_PORT, 0xff);
}
