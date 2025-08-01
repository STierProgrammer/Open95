[BITS 64]

global reload_gdt

section .text

reload_gdt:
    push 0x08
    push .reload_CS
    retfq

.reload_CS:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    xor ax, ax
    mov fs, ax
    mov gs, ax
    ret
