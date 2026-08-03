#include "multiboot.h"
#include <stdint.h>

static Multiboot2Info g_mbi;

static uint32_t align_up(uint32_t size) {
    return (size + 7) & ~7;
}

static uint8_t* get_next_tag(uint8_t* current_tag) {
    uint32_t tag_size = *(uint32_t*)(current_tag + 4);
    uintptr_t next = (uintptr_t)current_tag + align_up(tag_size);
    if (next >= (g_mbi.mbi_start + g_mbi.total_size)) {
        return 0;
    }
    return (uint8_t*)next;
}

void multiboot2_init(uint32_t magic, uint64_t addr) {
    g_mbi.mbi_start = (uintptr_t)addr;
    g_mbi.total_size = *(uint32_t*)addr;
    g_mbi.is_valid = (magic == 0x36d76289) && (g_mbi.total_size >= 8);
}

int multiboot2_is_valid(void) {
    return g_mbi.is_valid;
}

uintptr_t multiboot2_get_mbi_start(void) {
    return g_mbi.mbi_start;
}

uint32_t multiboot2_get_total_size(void) {
    return g_mbi.total_size;
}

void* multiboot2_find_tag(uint32_t type) {
    if (!g_mbi.is_valid) return 0;
    uint8_t* tag_ptr = (uint8_t*)(g_mbi.mbi_start + 8);
    uint8_t* end_ptr = (uint8_t*)(g_mbi.mbi_start + g_mbi.total_size);

    while (tag_ptr != 0 && tag_ptr < end_ptr) {
        uint32_t current_type = *(uint32_t*)tag_ptr;
        if (current_type == type) {
            return tag_ptr;
        }
        tag_ptr = get_next_tag(tag_ptr);
    }
    return 0;
}

void multiboot2_get_memory_map(const struct memory_map_entry** out_entries, uint32_t* out_count) {
    void* tag = multiboot2_find_tag(6);
    if (tag == 0) {
        *out_entries = 0;
        *out_count = 0;
        return;
    }

    uint8_t* data_ptr = (uint8_t*)tag + 16;
    *out_entries = (const struct memory_map_entry*)data_ptr;

    uint32_t tag_total_size = *(uint32_t*)((uint8_t*)tag + 4);
    uint32_t array_byte_size = tag_total_size - 16;
    uint32_t entry_size = *(uint32_t*)((uint8_t*)tag + 8);

    *out_count = (entry_size == 0) ? 0 : (array_byte_size / entry_size);
}