BITS 64

global loadGDT
global after_cs_reload

section .text
loadGDT:
    lgdt [rdi]

    push    0x08    
    ; lea     rax, [rel after_cs_reload] 
    ; push    rax
    push after_cs_reload
    retfq                       

after_cs_reload:
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    ret
