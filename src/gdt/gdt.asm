global loadGDT

section .text
loadGDT:
    lgdt [rdi]

    lea     rax, [rel .after_cs_reload] 
    push    rax       
    push    0x08                       
    retfq                               

.after_cs_reload:
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    ret
