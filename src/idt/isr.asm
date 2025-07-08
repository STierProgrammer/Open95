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

global   isr_wrapper
align   4

%macro isr_wrapper 1
    pushaq
    cld    ; C code following the sysV ABI requires DF to be clear on function entry
    call %1
    popaq
    iretq
%endmacro

global isr_divide_error
extern isr_divide_error_handler
isr_divide_error:
    isr_wrapper isr_divide_error_handler
