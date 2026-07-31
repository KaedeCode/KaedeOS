#include "multiboot.hpp"

void Multiboot2Info::check_magic(uint32_t magic) {
    is_valid = (magic == 0xE85250D6);
}

uint32_t Multiboot2Info::align_up(uint32_t size) {
    return (size + 7) & ~7;
}

uint8_t* Multiboot2Info::get_next_tag(uint8_t* current_tag) const {
    uint32_t tag_size = *reinterpret_cast<uint32_t*>(current_tag);
    uintptr_t next = reinterpret_cast<uintptr_t>(current_tag) + align_up(tag_size);
    if (next >= (mbi_start + total_size)) {
        return nullptr;
    }
    return reinterpret_cast<uint8_t*>(next);
}

Multiboot2Info::Multiboot2Info(uint32_t incoming_magic, uint64_t incoming_addr) 
    : mbi_start(static_cast<uintptr_t>(incoming_addr)),
      total_size(*reinterpret_cast<uint32_t*>(incoming_addr)),
      is_valid(false),
      current_tag_ptr(reinterpret_cast<uint8_t*>(incoming_addr) + 8)
{
    check_magic(incoming_magic);
    if (total_size < 8) {
        is_valid = false;
    }
}

void* Multiboot2Info::findTag(uint32_t type) const {
    uint8_t* tag_ptr = reinterpret_cast<uint8_t*>(mbi_start) + 8;
    uint8_t* end_ptr = reinterpret_cast<uint8_t*>(mbi_start + total_size);

    while (tag_ptr != nullptr && tag_ptr < end_ptr) {
        uint32_t current_type = *reinterpret_cast<uint32_t*>(tag_ptr);
        if (current_type == type) {
            return tag_ptr;
        }
        tag_ptr = get_next_tag(tag_ptr);
    }
    return nullptr;
}

void Multiboot2Info::getMemoryMap(const memory_map_entry** out_entries, uint32_t* out_count) const {
    void* tag = findTag(6); 
    if (tag == nullptr) {
        *out_entries = nullptr;
        *out_count = 0;
        return;
    }

    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(tag) + 16;
    *out_entries = reinterpret_cast<const memory_map_entry*>(data_ptr);

    uint32_t tag_total_size = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(tag) + 4);
    uint32_t array_byte_size = tag_total_size - 16;
    uint32_t entry_size = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(tag) + 8);
    
    if (entry_size == 0) {
        *out_count = 0;
    } else {
        *out_count = array_byte_size / entry_size;
    }
}