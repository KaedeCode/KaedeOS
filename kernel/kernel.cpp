#include <pmm.h>
#include <keyboard.h>
#include <vga.h>
#include <stdlib.h>
#include <stdint.h>

extern "C" void kernel_main_cpp() {
    asm volatile("sti");

    const char* msg = "KaedeOS C++ Kernel with PMM!";
    vga_write(msg, 0, 0, 0x0F);

    void* page = alloc_page();
    if (page == 0) {
        vga_write("alloc_page failed!", 1, 0, 0x0F);
        while(1) asm volatile("hlt");
    }

    char* buf = (char*)page;
    const char* test_str = "This is data in allocated page!";
    for (int i = 0; test_str[i] != '\0'; i++) {
        buf[i] = test_str[i];
    }
    buf[31] = '\0';

    vga_write(buf, 2, 0, 0x0E);

    char hexbuf[17];
    long_to_hex((unsigned long)page, hexbuf);
    vga_write("Page address: ", 3, 0, 0x0F);
    vga_write(hexbuf, 3, 15, 0x0F);

    vga_write("Press any key...", 4, 0, 0x0F);

    int col = 0;
    int row = 5;
    while (1) {
        char ch = read_char();
        if (ch != 0) {
            char str[2] = {ch, 0};
            vga_write(str, row, col, 0x0F);
            col++;
            if (col >= 80) {
                col = 0;
                row++;
            }
        }
        asm volatile("hlt");
    }
}