#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

struct memory_map_entry {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed));

typedef struct {
    uintptr_t mbi_start;
    uint32_t total_size;
    int is_valid;
} Multiboot2Info;

void multiboot2_init(uint32_t magic, uint64_t addr);

int multiboot2_is_valid(void);

uintptr_t multiboot2_get_mbi_start(void);
uint32_t multiboot2_get_total_size(void);

void* multiboot2_find_tag(uint32_t type);

void multiboot2_get_memory_map(const struct memory_map_entry** out_entries, uint32_t* out_count);

#endif