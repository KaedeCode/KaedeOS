#include <stdint.h>
#include <isr.h>
extern void (*idt_stubs_point[256])();

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
        unsigned long isp = (unsigned long)idt_stubs_point[i];
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