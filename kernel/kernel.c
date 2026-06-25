void kernel_main(void) {
    __asm__ __volatile__ ("sti");
    char *vga = (char *)0xb8000;
    const char *msg = "KaedeOS C Kernel";
    for (int i = 0; msg[i] != '\0'; i++) {
        vga[i * 2] = msg[i];
        vga[i * 2 + 1] = 0x0f;
    }
}