#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[no_mangle]
pub extern "C" fn kernel_main_rs() {
    let vga = 0xb8000 as *mut u8;
    let msg = b"KaedeOS Rust Kernel";
    for i in 0..msg.len() {
        unsafe {
            *vga.add(i * 2 + 80) = msg[i];
            *vga.add(i * 2 + 81) = 0x0c;
        }
    }
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}