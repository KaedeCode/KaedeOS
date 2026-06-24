extern функция_на_C_которуя_я_ещё_не_написал

%macro inter 1

section .text
    global inter%1
    inter%1:
        cmp %1, 8
        je no_err
        cmp %1, 10
        je no_err
        cmp %1, 11
        je no_err
        cmp %1, 12
        je no_err
        cmp %1, 13
        je no_err
        cmp %1, 14
        je no_err
        cmp %1, 17
        je no_err
        cmp %1, 21
        je no_err
        push 0
    .no_err:
        push R15
        push R14
        push R13
        push R12
        push R11
        push R10
        push R9
        push R8
        push RSP
        push RBP
        push RDI
        push RSI
        push RDX
        push RCX
        push RBX
        push RAX
        call функция_на_C_которуя_я_ещё_не_написал(rsp) ;Пока не знаю как эту функцию написать и передать, чуть позже займусь
        pop RAX
        pop RBX
        pop RCX
        pop RDX
        pop RSI
        pop RDI
        pop RBP
        pop RSP
        pop R8
        pop R9
        pop R10
        pop R11
        pop R12
        pop R13
        pop R14
        pop R15
        add rsp, 8
        iretq

section .data
idt_entry:
    dw (inter%1 & 0b1111111111111111)
    dw gdt64.code
    db 0
    db 0b10001110
    dw ((inter%1 >> 16) &  0xFFFF)
    dd (inter%1 >> 32)
    dd 0

