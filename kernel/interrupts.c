extern void vga_write(const char *s, int row, int col, unsigned char color);
extern void (*idt_stubs_point[256])();

//section .data
//    dw (inter%1 & 0b1111111111111111)
//    dw 0x08         ;gdt64.code
//    db 0
//    db 0b10001110
//    dw ((inter%1 >> 16) &  0xFFFF)
//    dd (inter%1 >> 32)
//    dd 0

struct ITD {
    unsigned short;
    unsigned short;
    unsigned char;
    unsigned char;
    unsigned short;
    unsigned int;
    unsigned int;
};

struct IDT IDT[256];

struct RegFrame {
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

struct ITD_pointer {
    unsigned short size;
    unsigned long pointer;
};

struct ITD_pointer data = { 0x0FFF, (unsigned long)idt_table_point };

void interrupts_handler(struct RegFrame *pointer) {
    for (int i = 0; i <= 255; i++) {
        IDT[i] = {
    unsigned short;
    unsigned short;
    unsigned char;
    unsigned char;
    unsigned short;
    unsigned int;
    unsigned int;
}
    }

    struct RegFrame *regs = pointer;
    regs->rflags = regs->rflags | (1 << 9);
    
    __asm__ __volatile__ (
        "lidt %0"
        :
        :"m"(data)
        :"memory"
    );
    __asm__ __volatile__ ("sti");

    vga_write("Opps...", 0, 0, 0x0F);
    while(1);
}