#ifndef FREE95_SERIAL_PORTS_H
#define FREE95_SERIAL_PORTS_H

#include <stdint.h>
#include <stdarg.h>
#include "arch/misc.h"

#define COM1_PORT 0x3f8

int init_serial();
int serial_received();
char read_serial();
int is_transmit_empty();

void srput(char a);
void srputs(const char* str);
void srprintf(const char* fmt, ...);

#endif
