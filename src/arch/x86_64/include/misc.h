#ifndef OPEN95_ARCH_MISC_H
#define OPEN95_ARCH_MISC_H

#include <stdint.h>

static inline void hcf(void)
{
  for (;;)
  {
    __asm__("hlt");
  }
}

static inline void outb(uint16_t port, uint8_t val)
{
  __asm__ volatile("outb %b0, %w1" ::"a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port)
{
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

#endif
