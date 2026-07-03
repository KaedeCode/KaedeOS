#pragma once

#include <isr.h>

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned long sec_now;
extern unsigned char is_extended;
extern unsigned int vga_col;

void keyboard_handler(RegFrame *frame);
void init_keyboard();

#ifdef __cplusplus
}
#endif