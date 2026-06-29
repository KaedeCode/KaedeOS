#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void vga_write(const char *s, int row, int col, unsigned char color);

#ifdef __cplusplus
}
#endif