#include "serial_ports.h"

static inline void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

int init_serial() {
  outb(PORT + 1, 0x00); // Disable all interrupts
  outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
  outb(PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial
                        // returns same byte)

  // Check if serial is faulty (i.e: not same byte as sent)
  if (inb(PORT + 0) != 0xAE) {
    return 1;
  }

  // If serial is not faulty set it in normal operation mode
  // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
  outb(PORT + 4, 0x0F);
  return 0;
}

int serial_received() { return inb(PORT + 5) & 1; }

char read_serial() {
  while (serial_received() == 0)
    ;

  return inb(PORT);
}

int is_transmit_empty() { return inb(PORT + 5) & 0x20; }

void srput(char a) {
  while (is_transmit_empty() == 0);

  outb(PORT, a);
}

void srputs(const char *str) {
  while (*str) {
    srput(*str);
    str++;
  }
}

// TODO: Add other specifiers
void srprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  while (*fmt) {
      if (*fmt == '%') {
          fmt++;
          switch (*fmt) {
              case 's': {
                  const char* value = va_arg(args, const char*);
                  srputs(value);
                  break;
              }
              case 'c': {
                  int value = va_arg(args, int);
                  srput((char)value);
                  break;
              }
              case 'x': {
                int value = va_arg(args, int);

                for (int i = 2 * sizeof(int); i > 0; i--) {
                    int new_val = (value >> ((i - 1) * 4)) & 0xF;

                    if (new_val < 10) {
                        srput(new_val + '0');
                    } else {
                        srput(new_val + 'A' - 10);
                    }
                }
              }
          }
      } else {
          srput(*fmt);
      }

      fmt++;
  }

  va_end(args);
}
