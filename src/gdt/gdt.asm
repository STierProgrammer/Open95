gdtr dw 0 
     dq 0

global setGDT
global reloadSegments

setGDT:
    mov [gdtr], di
    mov [gdtr+2], rsi
    lgdt [gdtr]
    ret

reloadSegments:
   ; reload cs register:
   push 0x08                 ; push code segment to stack, 0x08 is a stand-in for your code segment
   lea rax, [rel .reload_cs] ; load address of .reload_cs into rax
   push rax                  ; push this value to the stack
   retfq                     ; perform a far return, retfq or lretq depending on syntax

.reload_cs:
   ; reload data segment registers
   mov   ax, 0x10 ; 0x10 is a stand-in for your data segment
   mov   ds, ax
   mov   es, ax
   mov   fs, ax
   mov   gs, ax
   mov   ss, ax
   ret
