void interrupts_handler(int *pointer) {
    struct RegFrame
    {
        unsigned long RAX;
        unsigned long RBX;
        unsigned long RCX;
        unsigned long RDX;
        unsigned long RSI;
        unsigned long RDI;
        unsigned long RBP;
        unsigned long R8;
        unsigned long R9;
        unsigned long R10;
        unsigned long R11;
        unsigned long R12;
        unsigned long R13;
        unsigned long R14;
        unsigned long R15;
        unsigned long error_code;
        unsigned long rip;
        unsigned long cs;
        unsigned long rflags;
        unsigned long rsp;
        unsigned long ss;
    };

    struct RegFrame *regs = (struct RegFrame*)pointer;
    
}