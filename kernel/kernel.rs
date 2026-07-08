#![no_std]
#![no_main]

use core::panic::PanicInfo;

extern "C" {
    fn read_char() -> u8;
    fn vga_write(s: *const u8, row: i32, col: i32, color: u8);
}

#[no_mangle]
pub extern "C" fn kernel_main_rs() {
    unsafe { core::arch::asm!("sti"); }

    let msg = b"Keyboard Demo! Press any key...";
    unsafe {
        let vga_ptr = 0xb8000 as *mut u8;
        for i in 0..msg.len() {
            *vga_ptr.add(i * 2) = msg[i];
            *vga_ptr.add(i * 2 + 1) = 0x0f;
        }
    }

    let mut col = 0;
    loop {
        // 2. Читаем символ из буфера
        let ch = unsafe { read_char() };

        if ch != 0 {
            let s = [ch, 0];
            unsafe {
                vga_write(s.as_ptr(), 1, col, 0x0f);
            }
            col = (col + 1) % 80;
        }

        unsafe { core::arch::asm!("hlt"); }
    }
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {
        unsafe { core::arch::asm!("hlt"); }
    }
}