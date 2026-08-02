#include <stdint.h>
#include <stddef.h>

int strlen(const char *s) {
    int n = 0;
    while (*s++) n++;
    return n;
};

void *memset(void *dest, uint32_t value, size_t count) {
    uint8_t *ptr = (uint8_t*)dest;
    uint8_t val = (uint8_t)value;
    for(uint64_t i = 0; i < count; i++) {
        ptr[i] = val;
    };
    return dest;
}