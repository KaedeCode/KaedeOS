# Development Roadmap

Below is a step-by-step plan for building the operating system kernel, broken down by task, language, and status.

| # | Task | Language | Status |
|---|---|---|---|
| 0 | **Foundation** – boot via GRUB (Multiboot2), CPUID/long‑mode checks, transition to 64‑bit, initial page tables (L4/L3/L2 mapping 512 MiB), three entry points (C, C++, Rust) with VGA text output, basic I/O (`inb`/`outb`), `strlen`, Makefile/ISO build | – | ◼ |
| 1 | Interrupt Descriptor Table (IDT) and exception handlers (`#DE`, `#PF`, `#GP`, `#DF`) – write assembly stubs, register handlers, test with deliberate faults | C + asm | ◼ |
| 2 | Programmable Interrupt Controller (PIC) remapping and PIT timer (∼100 Hz) – enable IRQ0, increment ticks, print a message every second | C + asm | ◻ |
| 3 | PS/2 keyboard driver – read scancodes from port 0x60, translate to ASCII (US layout), store in circular buffer, echo to VGA | C | ◻ |
| 4 | Physical memory allocator – parse GRUB memory map, build bitmap (4 KiB pages), implement `alloc_page()` / `free_page()` | C | ◻ |
| 5 | Virtual memory manager – implement `kmalloc()`/`kfree()` with a simple heap (linked list or slab) using the physical allocator | C++ | ◻ |
| 6 | Context switching and round‑robin scheduler – define `task` structure, write `switch_context()` in assembly, create two kernel threads printing alternating messages | C++ + asm | ◻ |
| 7 | System calls – implement `int 0x80` handler with a `write(fd, buf, len)` call that prints to VGA/COM | C++ + asm | ◻ |
| 8 | Serial port (COM1) driver – initialise baud rate 115200, implement `serial_write()` and `serial_print()` for off‑screen debugging | Rust | ◻ |
| 9 | RAM disk (tar) support – load a tar archive via GRUB module, parse headers, implement `read_file(name)` returning a pointer to file data | Rust | ◻ |
| 10 | ELF loader and first user‑mode program – load a simple ELF binary from the tar archive, set up user address space, switch to ring 3 and execute, using the `write` syscall | Rust | ◻ |