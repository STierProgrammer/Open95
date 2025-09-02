#ifndef OPEN95_DEVICES_PIC_H
#define OPEN95_DEVICES_PIC_H

#include <stdint.h>
#include "misc.h"

#define MASTER_PIC_COMMAND_PORT 0x20
#define MASTER_PIC_DATA_PORT 0x21
#define SLAVE_PIC_COMMAND_PORT 0xA0
#define SLAVE_PIC_DATA_PORT 0xA1

#define CMD_PIC_EOI 0x20

void pic_send_eoi(uint8_t irq);
void pic_disable(void);
#endif