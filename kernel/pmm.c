#include <string.h>
#include <stdint.h>
#include <multiboot.h>
#include <pmm.h>

uint8_t pmm_bitmap[BITMAP_SIZE];

static inline void set_bit(uint64_t page) {
    uint64_t idx = page / 8;
    if (idx < BITMAP_SIZE) {
        pmm_bitmap[idx] |= (1 << (page % 8));
    }
}

static inline void clear_bit(uint64_t page) {
    uint64_t idx = page / 8;
    if (idx < BITMAP_SIZE) {
        pmm_bitmap[idx] &= ~(1 << (page % 8));
    }
}

void pmm_init() {
    memset(pmm_bitmap, 0xFF, BITMAP_SIZE);

    const struct memory_map_entry* entries;
    uint32_t entry_count;
    multiboot2_get_memory_map(&entries, &entry_count);

    for (uint32_t i = 0; i < entry_count; i++) {
        const struct memory_map_entry* entry = &entries[i];
        if (entry->type == 1) {
            uint64_t start = (entry->base_addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
            uint64_t end = (entry->base_addr + entry->length) & ~(PAGE_SIZE - 1);
            for (uint64_t addr = start; addr < end; addr += PAGE_SIZE) {
                clear_bit(addr / PAGE_SIZE);
            }
        }
    }

    uint64_t kernel_start = (uint64_t)_start / PAGE_SIZE;
    uint64_t kernel_end = ((uint64_t)_end + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint64_t p = kernel_start; p < kernel_end; p++) {
        set_bit(p);
    }

    uint64_t bitmap_start = (uint64_t)pmm_bitmap / PAGE_SIZE;
    uint64_t bitmap_end = ((uint64_t)pmm_bitmap + BITMAP_SIZE + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint64_t p = bitmap_start; p < bitmap_end; p++) {
        set_bit(p);
    }

    set_bit(0);
}

void* alloc_page(void) {
    uint64_t total_pages = BITMAP_SIZE * 8;
    for (uint64_t p = 0; p < total_pages; p++) {
        uint64_t idx = p / 8;
        uint8_t bit = p % 8;
        if ((pmm_bitmap[idx] & (1 << bit)) == 0) {
            pmm_bitmap[idx] |= (1 << bit);
            return (void*)(p * PAGE_SIZE);
        }
    }
    return 0;
}

void free_page(void* addr) {
    uint64_t p = (uint64_t)addr / PAGE_SIZE;
    uint64_t idx = p / 8;
    uint8_t bit = p % 8;
    if (idx < BITMAP_SIZE) {
        pmm_bitmap[idx] &= ~(1 << bit);
    }
}