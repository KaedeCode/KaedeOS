void vga_write(const char *s, int row, int col, unsigned char color) {
    char *vga = (char *)0xb8000;
    int offset = (row * 80 + col) * 2;
    for (int i = 0; s[i] != '\0'; i++) {
        vga[offset + i * 2] = s[i];
        vga[offset + i * 2 + 1] = color;
    }
}