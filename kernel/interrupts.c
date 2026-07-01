#include <isr.h>

isr_handler_type dispatch_table[256];

void init_interrupts() {
    init_idt();
    init_exceptions();
    init_pit();
};

void register_isr(int vector, isr_handler_type handler) {
    dispatch_table[vector] = handler;
};

void interrupts_handler(struct RegFrame *pointer) {
    dispatch_table[pointer->vector](pointer);
}