#include "limine.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "include/misc.h"
#include "devices/include/serial.h"
#include "gdt/include/gdt.h"
#include "idt/include/idt.h"
#include "mem/include/pmm.h"
#include "mem/include/paging.h"
#include "mem/include/kheap.h"
#include "bootlayer/include/bootlayer.h"

struct PageTable* pml4 = NULL;

// extern volatile struct limine_framebuffer_request framebuffer_request;

void kmain(void)
{
    init_serial();
    init_gdt();
    init_idt();
    pmm_init();

    init_pml4();
    map_kernel();
    map_memmap();
    set_cr3((uint64_t)(((uint64_t)pml4) - get_hhdm()));

    init_kheap();
    kmalloc(50);
    void* y = kmalloc(30);
    kfree(y);
    krealloc(y, 30);
    print_kheap();

    // if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    // {
    //     hcf();
    // }

    // struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // for (size_t i = 0; i < 100; i++)
    // {
    //     volatile uint32_t *fb_ptr = framebuffer->address;

    //     fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xff0000;
    // }

    hcf();
}
