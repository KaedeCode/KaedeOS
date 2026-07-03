#include <ports.h>
#include <vga.h>
#include <isr.h>
#include <pic.h>
#include <stdlib.h>

static volatile unsigned char pit_ticks = 0;
static volatile unsigned long pit_sec = 0;

void pit_handler(RegFrame *frame) {
    pit_ticks++;
    if (pit_ticks == 100) {
        pit_sec++;
        pit_ticks = 0;
    };
    outb(0x20, 0x20);
};

long get_second(void) {
    return pit_sec;
};

void init_pit() {
    outb(0x43, 0x36);
    outb(0x40, 11931);
    outb(0x40, 11931 >> 8);
    ICW();
    enable_pic(0);
    register_isr(0x20, pit_handler);
};