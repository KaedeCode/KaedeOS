extern void vga_write(const char *s, int row, int col, unsigned char color);
extern void (*idt_stubs_point[256])();
#include <stdint.h>

struct __attribute__((packed)) IDT_struct {
    unsigned short offset_low;
    unsigned short segment_selector;
    unsigned char ist_flags;
    unsigned char type_attributes;
    unsigned short offset_mid;
    unsigned int offset_high;
    unsigned int reserved;
};

struct __attribute__((packed)) IDT_pointer {
    unsigned short size;
    unsigned long pointer;
};

struct IDT_struct IDT[256];

void init_idt() {
    for (int i = 0; i <= 255; i++) {
        uint64_t isp = (uint64_t)idt_stubs_point[i];
        IDT[i].offset_low = (isp & 0b1111111111111111);
        IDT[i].segment_selector = 0x08;
        IDT[i].ist_flags = 0;
        IDT[i].type_attributes = 0b10001110;
        IDT[i].offset_mid = ((isp >> 16) &  0xFFFF);
        IDT[i].offset_high = (isp >> 32);
        IDT[i].reserved = 0;
    };

    struct IDT_pointer data = { 0x0FFF, (unsigned long)&IDT };

    __asm__ __volatile__ (
        "lidt %0"
        :
        :"m"(data)
        :"memory"
    );
};

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

void interrupts_handler(struct RegFrame *pointer) {
    struct RegFrame *regs = pointer;
    regs->rflags = regs->rflags | (1 << 9);

    vga_write("Opps...", 0, 0, 0x0F);
    while(1);
}