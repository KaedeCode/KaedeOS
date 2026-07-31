#pragma once

#include <stdint.h>

#define BITMAP_SIZE (128 * 1024)
#define PAGE_SIZE 4096
#define PAGE_SHIFT 12

extern uint8_t _start[];
extern uint8_t _end[];
extern uint8_t pmm_bitmap[BITMAP_SIZE];

void pmm_init(uint64_t mbi_addr);
void* alloc_page(void);
void free_page(void* addr);