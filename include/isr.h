#ifndef ISR_H
#define ISR_H


struct RegFrame {
    unsigned long rax, rbx, rcx, rdx, rsi, rdi, rbp;
    unsigned long r8, r9, r10, r11, r12, r13, r14, r15;
    unsigned long error_code;
    unsigned long vector;
    unsigned long rip, cs, rflags, rsp, ss;
};

typedef void(*isr_handler_type)(struct RegFrame*);

extern isr_handler_type dispatch_table[256];

void register_isr(int vector, isr_handler_type handler);

void init_idt(void);
void init_exceptions(void);
void init_pit(void);
void ICW(void);

#endif