#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "misc.h"
#include "devices/serial.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "mem/pmm.h"
#include "mem/paging.h"
#include "mem/kheap.h"
#include "bootstub.h"

struct PageTable* pml4;
struct KernelParams* krnl_params;

void kmain(struct KernelParams* kernel_params)
{
    krnl_params = kernel_params;
    
    init_serial();
    init_gdt();
    init_idt();
    pmm_init(kernel_params);

    init_pml4();
    map_kernel();
    map_memmap();
    set_cr3((uint64_t)(((uint64_t)pml4) - kernel_params->hhdm));

    init_kheap();
    kmalloc(50);
    void* y = kmalloc(30);
    kfree(y);
    krealloc(y, 30);
    print_kheap();

    hcf();
}
