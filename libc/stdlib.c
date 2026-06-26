void long_to_hex(unsigned long num, char *buffer) {
    static char const hnum[] = "0123456789ABCDEF";
    buffer[16] = '\0';
    for (char i = 15; i >= 0; i--) {
        buffer[i] = hnum[num & 0xF];
        num >>= 4;
    };
}