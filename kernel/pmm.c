#include <string.h>
#include <stdint.h>
#include <pmm.h>

void pmm_init(uint64_t mbi_addr) {
    memset(pmm_bitmap, 0xFF, BITMAP_SIZE);
    
};