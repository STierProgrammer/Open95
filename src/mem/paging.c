#include "paging.h"
#include <stdint.h>

// uintptr_t getPhysicalAddress(PageTable* cr3, uintptr_t virtual_addr) {
//     uint64_t offset = virtual_addr & 0xFFF;
//     uint64_t pml1_index = (virtual_addr >> 12) & 0x1FF;
//     uint64_t pml2_index = (virtual_addr >> 21) & 0x1FF;
//     uint64_t pml3_index = (virtual_addr >> 30) & 0x1FF;
//     uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
// }
