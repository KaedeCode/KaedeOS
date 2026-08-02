#include "multiboot.h"

static uint32_t align_up(uint32_t size) {
    return (size + 7) & ~7;
}

static uint8_t* get_next_tag(const Multiboot2Info* info, uint8_t* current_tag) {
    uint32_t tag_size = *(uint32_t*)current_tag;
    uintptr_t next = (uintptr_t)current_tag + align_up(tag_size);
    if (next >= (info->mbi_start + info->total_size)) {
        return 0;
    }
    return (uint8_t*)next;
}

void multiboot2_init(Multiboot2Info* info, uint32_t magic, uint64_t addr) {
    info->mbi_start = (uintptr_t)addr;
    info->total_size = *(uint32_t*)addr;
    info->is_valid = (magic == 0xE85250D6);
    if (info->total_size < 8) {
        info->is_valid = 0;
    }
}

void* multiboot2_find_tag(const Multiboot2Info* info, uint32_t type) {
    uint8_t* tag_ptr = (uint8_t*)(info->mbi_start + 8);
    uint8_t* end_ptr = (uint8_t*)(info->mbi_start + info->total_size);

    while (tag_ptr != 0 && tag_ptr < end_ptr) {
        uint32_t current_type = *(uint32_t*)tag_ptr;
        if (current_type == type) {
            return tag_ptr;
        }
        tag_ptr = get_next_tag(info, tag_ptr);
    }
    return 0;
}

void multiboot2_get_memory_map(const Multiboot2Info* info, const struct memory_map_entry** out_entries, uint32_t* out_count) {
    void* tag = multiboot2_find_tag(info, 6);
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

    if (entry_size == 0) {
        *out_count = 0;
    } else {
        *out_count = array_byte_size / entry_size;
    }
}