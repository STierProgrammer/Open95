#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "misc.h"
#include "devices/serial.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "mem/pmm.h"
#include "paging/paging.h"
#include "mem/kheap.h"
#include "bootstub.h"

struct PageTable* pml4;
struct KernelParams* kernel_params;

void kmain(struct KernelParams* krnl_params)
{
    kernel_params = krnl_params;
    
    init_serial();
    init_gdt();
    init_idt();
    pmm_init();

    init_pml4();
    map_kernel();
    map_memmap();
    set_cr3((uint64_t)(((uint64_t)pml4) - kernel_params->hhdm));

    hcf();
}
