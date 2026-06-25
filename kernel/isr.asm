extern interrupts_handler

%macro inter 1
global inter%1
inter%1:
    %if %1 == 8 || %1 == 10 || %1 == 11 || %1 == 12 || %1 == 13 || %1 == 14 || %1 == 17 || %1 == 21
    %else
        push 0
    %endif

    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax

    mov rdi, rsp
    call interrupts_handler

    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15

    add rsp, 8
    iretq
%endmacro

section .text
%assign i 0
%rep 256
    inter %[i]
%assign i i + 1
%endrep

%macro generate_stub_ptr 1
    dq inter%1
%endmacro

section .data
global idt_stubs_point
idt_stubs_point:
%assign i 0
%rep 256
    generate_stub_ptr %[i]
%assign i i + 1
%endrep