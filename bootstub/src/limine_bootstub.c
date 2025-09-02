#include "bootstub.h"
#include <stddef.h>
#include "limine.h"

#define MAX_HEAP_SIZE 1024 * 16

static struct KernelParams kernel_params;
static char heap[MAX_HEAP_SIZE];
static uint32_t heap_head = 0;

static void* heap_alloc(size_t size) 
{
    size = (size + 0xF) & ~0xF;
    if (!size || size > MAX_HEAP_SIZE - heap_head) return NULL;
    
    heap_head += size;
    return (void*)(heap + heap_head - size);
}

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);
__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".limine_requests"))) 
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 3};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 3};

__attribute__((used, section(".limine_requests"))) 
static volatile struct limine_executable_address_request kernel_address_request = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = 3};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 3};

static int from_limine_type(int type) 
{
    switch (type) 
    {
        case LIMINE_MEMMAP_USABLE: return MEMMAP_USABLE;
        case LIMINE_MEMMAP_RESERVED: return MEMMAP_RESERVED;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE: return MEMMAP_ACPI_RECLAIMABLE;
        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES: return MEMMAP_EXECUTABLE_AND_MODULES;
        case LIMINE_MEMMAP_ACPI_NVS: return MEMMAP_ACPI_NVS;
        case LIMINE_MEMMAP_BAD_MEMORY: return MEMMAP_BAD_MEMORY;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE: return MEMMAP_BOOTLOADER_RECLAIMABLE;
        case LIMINE_MEMMAP_FRAMEBUFFER: return MEMMAP_FRAMEBUFFER;
        default: return MEMMAP_UNKNOWN;
    }
}

static struct KernelAddress get_kernel_address(void) 
{
    struct KernelAddress kaddr;
    kaddr.physical_base = kernel_address_request.response->physical_base;
    kaddr.virtual_base = kernel_address_request.response->virtual_base;

    return kaddr;
}

static struct Memmap get_memmap(void)
{
    struct Memmap memmap;
    memmap.entry_count = memmap_request.response->entry_count;
    memmap.entries = heap_alloc(memmap.entry_count * sizeof(struct MemmapEntry));
    for (uint64_t i = 0; i < memmap.entry_count; i++)
    {
        memmap.entries[i].base = memmap_request.response->entries[i]->base;
        memmap.entries[i].length = memmap_request.response->entries[i]->length;
        memmap.entries[i].type = from_limine_type(memmap_request.response->entries[i]->type);
    }

    return memmap;
}

static struct Framebuffer* get_framebuffers(void)
{
    struct Framebuffer* framebuffers = heap_alloc(framebuffer_request.response->framebuffer_count * sizeof(struct Framebuffer));
    for (uint64_t i = 0; i < framebuffer_request.response->framebuffer_count; i++)
    {
        framebuffers[i].addr = (uint64_t)framebuffer_request.response->framebuffers[i]->address;
        framebuffers[i].width = framebuffer_request.response->framebuffers[i]->width;
        framebuffers[i].height = framebuffer_request.response->framebuffers[i]->height;
        framebuffers[i].pitch = framebuffer_request.response->framebuffers[i]->pitch;
        framebuffers[i].memory_model = framebuffer_request.response->framebuffers[i]->memory_model;
    }

    return framebuffers;
}

static void load_kernel_params()
{
    kernel_params.hhdm = hhdm_request.response->offset; 
    kernel_params.kernel_addr = get_kernel_address();
    kernel_params.memmap = get_memmap();
    kernel_params.framebuffers = get_framebuffers();
}

extern void kmain(struct KernelParams* kernel_params);

void kstart(void) 
{
    load_kernel_params();
    kmain(&kernel_params);
}