# KaedeOS

**KaedeOS** is my educational 64‑bit operating system written from scratch.  
The project is created for a deep understanding of computer operation: from booting to memory management and I/O devices.  
Currently the kernel can boot via GRUB, switch to long mode, handle CPU exceptions, run a programmable interval timer, handle keyboard input via PS/2, and manage physical memory through a bitmap allocator.

## Screenshots

![Three language entry points](screenshots/entry_points.png)  
*C (white), C++ (yellow), and Rust (red) entry points – each prints its own message with a distinct colour.*

![PIT timer counter](screenshots/timer.png)  
*PIT timer running: seconds counter displayed on VGA (0x0F white on black).*

![CPU exception handler](screenshots/exception.png)  
*Divide-by-zero exception (#DE) caught – full register dump printed to VGA.*

![Keyboard input demo](screenshots/keyboard_input.png)
*PS/2 keyboard driver in action – typing echoes characters to the VGA buffer in real time.*

![PMM allocation demo](screenshots/pmm.png)
*Physical Memory Manager (PMM) demo – a physical page is allocated, a test string is written into it, and its address (in hex) is displayed on the VGA screen.*

## Project Structure

```
KaedeOS/
├── bootloader/
│ ├── boot.asm
│ └── multiboot_header.asm
├── drivers/
│ ├── vga.c
│ ├── pit.c
│ └── keyboard.c
├── kernel/
│ ├── kernel.c
│ ├── kernel.cpp
│ ├── kernel.rs
│ ├── isr.asm
│ ├── idt.c
│ ├── exceptions.c
│ ├── interrupts.c
│ ├── pic.c
│ ├── multiboot.c # Multiboot2 parser
│ └── pmm.c # Physical Memory Manager
├── libc/
│ ├── string.c
│ └── stdlib.c
├── utils/
│ └── ports.c
├── include/
│ ├── ports.h
│ ├── vga.h
│ ├── stdlib.h
│ ├── isr.h
│ ├── pic.h
│ ├── pit.h
│ ├── keyboard.h
│ ├── multiboot.h
│ ├── pmm.h
│ └── string.h
├── .gitignore
├── LICENSE
├── Makefile
├── README.md
├── ROADMAP.md
├── CHANGELOG.md
└── linker.ld
```

## Current Features

- **Multiboot2 boot** – boots via GRUB with a valid header.
- **CPU checks** – verifies CPUID and long‑mode support; sets up initial page tables (L4/L3/L2 mapping 512 MiB).
- **64‑bit long mode** – successfully transitions to 64‑bit mode with a custom GDT.
- **Three language entry points** – C, C++, and Rust each print their own message with a distinct colour (white, yellow, red) directly to VGA.
- **VGA text driver** – `vga_write()` with configurable row, column, and colour.
- **I/O utilities** – `inb()` / `outb()` for port access.
- **Minimal libc** – `strlen()`, `long_to_hex()` for hexadecimal conversion, and `get_second()` for PIT time.
- **Full interrupt subsystem**:
  - **IDT** with 256 interrupt gates, correctly handling error codes (dummy push when needed).
  - **Assembly stubs** save and restore all general‑purpose registers; call the C dispatcher.
  - **CPU exception handlers** (vectors 0‑31) – print all registers and halt on fault.
  - **Programmable Interrupt Controller (PIC)** – remapped to vectors 0x20 (master) and 0x28 (slave).
  - **PIT timer** – configured to ~100 Hz; increments a seconds counter and displays it on screen.
  - **PS/2 keyboard driver** – reads scancodes from port `0x60`, handles extended sequences, translates to ASCII (US layout), stores in a circular buffer, and supports modifiers (Shift, Caps, Ctrl, Alt).
  - **Modular registration API** – any driver can register its handler via `register_isr(vector, handler)`.
- **Physical Memory Manager (PMM)** – parses the Multiboot2 memory map, maintains a bitmap of free 4 KiB pages, and provides `alloc_page()` / `free_page()` for the kernel.
- **Multiboot2 support** – reads the memory map and other tags provided by the bootloader, making them available to kernel modules.
- **Centralised initialisation** – `init_interrupts()` sets up IDT, exceptions, PIC, PIT, and keyboard in one call.

### Requirements

- `make`
- `nasm` ≥ 2.14
- `x86_64-elf-gcc`, `x86_64-elf-g++`
- `rustc` with target `x86_64-unknown-none`
- `grub-mkrescue` and `xorriso`

### Build and Run

```bash
make          # build the ISO image (build/kaedeos.iso)
make run      # run in QEMU (options: -m 512M -smp 2)
make clean    # clean temporary files
```

> **View the Project Roadmap:** [ROADMAP.md](ROADMAP.md)