#include <ports.h>
#include <pic.h>

void ICW() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0xFA);
    outb(0xA1, 0xFF);
};

void enable_pic(unsigned char pic) {
    if (pic < 8) {
        master_mask &= ~(1 << pic);
    }
    else {
        slave_mask &= ~(1 << (pic - 8));
    }
};

unsigned char master_mask = 0xFF;
unsigned char slave_mask = 0xFF;