extern "C" void kernel_main_cpp() {
    char *vga = (char *)0xb8000;
    const char *msg = "KaedeOS C++ Kernel";
    for (int i = 0; msg[i] != '\0'; i++) {
        vga[i * 2 + 40] = msg[i];
        vga[i * 2 + 41] = 0x0e;
    }
}