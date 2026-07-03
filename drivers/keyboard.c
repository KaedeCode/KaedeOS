#include <ports.h>
#include <isr.h>
#include <vga.h>
#include <pit.h>
#include <stdlib.h>
#include <pic.h>

unsigned char ascii_map[128] = { 0 };
unsigned char ench_map[128] = { 0 };

void init_maps() {
    ascii_map[0x01] = '\e';
    ascii_map[0x0E] = '\b';
    ascii_map[0x1C] = '\n';
    ascii_map[0x0F] = '\t';
    ascii_map[0x39] = ' ';
    ascii_map[0x29] = '`';
    ascii_map[0x02] = '1';
    ascii_map[0x03] = '2';
    ascii_map[0x04] = '3';
    ascii_map[0x05] = '4';
    ascii_map[0x06] = '5';
    ascii_map[0x07] = '6';
    ascii_map[0x08] = '7';
    ascii_map[0x09] = '8';
    ascii_map[0x0A] = '9';
    ascii_map[0x0B] = '0';
    ascii_map[0x0C] = '-';
    ascii_map[0x0D] = '=';
    ascii_map[0x10] = 'q';
    ascii_map[0x11] = 'w';
    ascii_map[0x12] = 'e';
    ascii_map[0x13] = 'r';
    ascii_map[0x14] = 't';
    ascii_map[0x15] = 'y';
    ascii_map[0x16] = 'u';
    ascii_map[0x17] = 'i';
    ascii_map[0x18] = 'o';
    ascii_map[0x19] = 'p';
    ascii_map[0x1A] = '[';
    ascii_map[0x1B] = ']';
    ascii_map[0x1E] = 'a';
    ascii_map[0x1F] = 's';
    ascii_map[0x20] = 'd';
    ascii_map[0x21] = 'f';
    ascii_map[0x22] = 'g';
    ascii_map[0x23] = 'h';
    ascii_map[0x24] = 'j';
    ascii_map[0x25] = 'k';
    ascii_map[0x26] = 'l';
    ascii_map[0x27] = ';';
    ascii_map[0x28] = "'";
    ascii_map[0x2B] = "\\";
    ascii_map[0x2C] = 'z';
    ascii_map[0x2D] = 'x';
    ascii_map[0x2E] = 'c';
    ascii_map[0x2F] = 'v';
    ascii_map[0x30] = 'b';
    ascii_map[0x31] = 'n';
    ascii_map[0x32] = 'm';
    ascii_map[0x33] = ',';
    ascii_map[0x34] = '.';
    ascii_map[0x35] = '/';
}

unsigned long sec_now = 0;
unsigned char is_extended = 0;
int vga_col = -1;

void keyboard_handler(RegFrame *frame) {
    if (vga_col > 24) { 
        vga_col = -1; 
        for(int i = 0; i < 4000; i++) {
            char *vga = (char *)0xb8000;
            vga[i] = 0;
        }
    };
    if ((get_second() - sec_now) >= 2) {
        is_extended = 0;
        vga_col++;
        char buffer[17];
        long_to_hex(inb(0x60), buffer);
        vga_write(&buffer[14], vga_col, 0, 0x0F);
    }
    else {
        is_extended++;
        char buffer[17];
        long_to_hex(inb(0x60), buffer);
        vga_write(&buffer[14], vga_col, is_extended * 3, 0x0F);
    }
    outb(0x20, 0x20);
    sec_now = get_second();
};

void init_keyboard() {
    enable_pic(1);
    register_isr(0x21, keyboard_handler);
};