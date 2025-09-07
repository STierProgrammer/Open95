#include "serial.h"

int init_serial()
{
    outb(COM1_PORT + 1, 0x00); // Disable all interrupts
    outb(COM1_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(COM1_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1_PORT + 1, 0x00); //                  (hi byte)
    outb(COM1_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(COM1_PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(COM1_PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (inb(COM1_PORT + 0) != 0xAE)
    {
        return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(COM1_PORT + 4, 0x0F);

    srprintf("[Serial Initialized]\n");
    
    return 0;
}

int serial_received()
{
    return inb(COM1_PORT + 5) & 1;
}

char read_serial()
{
    while (serial_received() == 0)
        ;
    return inb(COM1_PORT);
}

int is_transmit_empty()
{
    return inb(COM1_PORT + 5) & 0x20;
}

void srput(char a)
{
    while (is_transmit_empty() == 0)
        ;

    outb(COM1_PORT, a);
}

void srputs(const char *str)
{
    while (*str)
    {
        srput(*str);
        str++;
    }
}

// TODO: Add other specifiers
void srprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
            case 's':
            {
                const char *value = va_arg(args, const char *);
                srputs(value);
                break;
            }
            case 'c':
            {
                int value = va_arg(args, int);
                srput((char)value);
                break;
            }
            case 'd':
            {
                int value = va_arg(args, int);
                
                char str[64] = {0};
                int i = 0;
                while (value > 0)
                {
                    char digit = value % 10 + '0';
                    value /= 10;
                    str[i++] = digit;
                }

                for (int i = 63; i >= 0; i--) {
                    srput(str[i]);
                }

                break;
            }
            case 'b': {
                int value = va_arg(args, int);

                if (value == 0) {
                    srputs("false");
                } else if (value == 1) {
                    srputs("true");
                } else {
                    srputs("corrupted");
                }

                break;
            }
            case 'x':
            {
                uint64_t value = va_arg(args, uint64_t);

                for (int i = 2 * sizeof(uint64_t); i > 0; i--)
                {
                    int new_val = (value >> ((i - 1) * 4)) & 0xF;

                    if (new_val < 10)
                    {
                        srput(new_val + '0');
                    }
                    else
                    {
                        srput(new_val + 'A' - 10);
                    }
                }

                break;
            }
            case '%':
            {
                srput('%');

                break;
            }
            }
        }
        else
        {
            srput(*fmt);
        }

        fmt++;
    }

    va_end(args);
}
