#ifndef MULTIBOOT2_HPP
#define MULTIBOOT2_HPP

#include <stdint.h>

struct memory_map_entry {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed));

class Multiboot2Info {
private:
    uintptr_t mbi_start;
    uint32_t total_size;
    bool is_valid;
    uint8_t* current_tag_ptr;

    void check_magic(uint32_t magic);
    static uint32_t align_up(uint32_t size);
    uint8_t* get_next_tag(uint8_t* current_tag) const;

public:
    Multiboot2Info(uint32_t incoming_magic, uint64_t incoming_addr);

    inline bool isValid() const { return is_valid; }
    inline uint32_t getTotalSize() const { return total_size; }

    void* findTag(uint32_t type) const;
    void getMemoryMap(const memory_map_entry** out_entries, uint32_t* out_count) const;
};

#endif