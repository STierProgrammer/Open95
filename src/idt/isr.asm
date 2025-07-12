%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popaq 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

global isr_divide_error
isr_divide_error:
    push 0
    push 0x00
    jmp isr_exception

global isr_invalid_opcode
isr_invalid_opcode:
    push 0
    push 0x06
    jmp isr_exception

global isr_double_fault
isr_double_fault:
    push 0
    push 0x08
    jmp isr_exception

global isr_exception
extern isr_exception_handler
isr_exception:
   pushaq
   mov rdi, rsp
   cld
   call isr_exception_handler
   popaq
   add rsp, 16
   iretq
