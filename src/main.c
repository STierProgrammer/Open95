#include "limine.h"
#include "misc.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "gdt/gdt.h"
#include "idt/idt.h"
#include "mem/pmm.h"
#include "devices/serial.h"
#include "devices/ps2.h"
#include "mem/paging.h"

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.
__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

void kmain(void)
{
    init_serial();
    srprintf("[Serial Initialized]\n");

    initGDT();
    srprintf("[GDT Initialized]\n");

    initIDT();
    srprintf("[IDT Initialized]\n");

    pmmInit();
    srprintf("[PMM Initialized]\n");

    uint8_t res = ps2_test_controller();
    srprintf("PS2 Controller Test: %x\n", res);
    
    uint64_t prev = free_mem_head->base;
    srprintf("%x\n", prev);
    palloc();
    srprintf("%x\n", free_mem_head->base);
    pfree(prev);    
    srprintf("%x\n", free_mem_head->base);

    PageTable* pml4 = init_pml4();
    srprintf("pml4: %x\n", pml4);
    map_page_table(pml4, (uint64_t)0x100000, (uint64_t)0xffff800000100000, PAGE_PRESENT | PAGE_WRITABLE);
    

    if (LIMINE_BASE_REVISION_SUPPORTED == false || framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 0; i < 100; i++)
    {
        volatile uint32_t *fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xff0000;
    }

    hcf();
}
