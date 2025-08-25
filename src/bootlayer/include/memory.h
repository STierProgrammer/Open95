#ifndef BOOTLAYER_MEMORY_H
#define BOOTLAYER_MEMORY_H

#include <stdint.h>
#include <limine.h>

extern volatile struct limine_hhdm_request hhdm_request;
extern volatile struct limine_memmap_request memmap_request;
extern volatile struct limine_executable_address_request kernel_address_request;

static inline uint64_t get_hhdm_offset(void) {
    return hhdm_request.response->offset; 
}

static inline struct limine_memmap_response *get_memmap(void) {
    return memmap_request.response;
}

static inline struct limine_memmap_entry *get_memmap_entry(uint64_t index) {
    return get_memmap()->entries[index];
}

static inline struct limine_executable_address_response* get_kernel_address(void) {
    return kernel_address_request.response;
}

#endif