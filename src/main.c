#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../limine/limine.h"
#include "arch/misc.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "serial_ports/serial_ports.h"

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.
__attribute__((used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.
__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.
__attribute__((used, section(".limine_requests_start"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;

void kmain(void)
{
    init_serial();
    initGDT();
    srprintf("GDT Initialized\n");

    initIDT();
    srprintf("IDT Initialized\n");

    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false)
    {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
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
