#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

int strlen(const char *s);

void *memset(void *dest, uint32_t value, size_t count);

#endif