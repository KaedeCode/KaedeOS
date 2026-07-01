#include <isr.h>
extern void vga_write(const char *s, int row, int col, unsigned char color);
extern void long_to_hex(unsigned long num, char *buffer);
const char *FrameName[] = { "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "R8", "R9", "R10", "R11", "R12","R13", "R14", "R15", "err", "vec", "rip", "cs", "rfl", "rsp", "ss" };

void exception_handler(struct RegFrame *pointer) {
    volatile struct RegFrame *regs = pointer;
    for(unsigned char i = 0; i != 21; i++) {
        unsigned long value = *(((unsigned long*)regs) + i);
        vga_write(FrameName[i], i, 0, 0x0f);
        vga_write(":", i, 3, 0x0f);
        char buffer[17];
        long_to_hex(value, buffer);
        vga_write(buffer, i, 5, 0x0f);
    };
    while(1);
}

void init_exceptions() {
    for (int i = 0; i <= 31; i++) {
        register_isr(i, exception_handler);
    };
}