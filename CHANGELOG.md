# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 2026-06-21

### Added
- Initial bootable kernel with Multiboot2 header (GRUB support).
- CPUID and long‑mode capability checks during early boot.
- Page table setup (L4, L3, L2) and successful transition to 64‑bit long mode.
- Three separate entry points written in C, C++, and Rust – each prints its own message to the VGA buffer with a distinct color (white, yellow, red).
- Basic VGA text output driver with configurable row, column, and color.
- I/O port utilities: `inb()`, `outb()`.
- Simple string length function (`strlen`) in the minimal libc.
- Makefile build system:
  - `make` – compiles the kernel and creates a bootable ISO image (`build/kaedeos.iso`).
  - `make run` – launches the ISO in QEMU with 512 MB RAM and 2 CPU cores.
  - `make clean` – removes temporary build artifacts.
- Project structure with separate directories for bootloader, kernel, drivers, libc, and utilities.
- Linker script that places the multiboot header at the 1 MiB mark and produces a 64‑bit ELF executable.

### Changed
- (None – initial release)

### Deprecated
- (None)

### Removed
- (None)

### Fixed
- (None)

### Security
- (None)

## [0.2.0] - 2026-06-24

### Added
- Implement Interrupt Descriptor Table (IDT) infrastructure and interrupt service routine (ISR) stubs via a new `isr.asm` file.
- Add a NASM macro that generates 256 ISR stubs for all exception vectors (0–255).
- Implement correct error-code handling for exceptions:
  - Macro automatically pushes a dummy `0` onto the stack for exceptions without an error code.
  - Skips the dummy push for exceptions that already have an error code pushed by the CPU (`#DF`, `#TS`, `#NP`, `#SS`, `#GP`, `#PF`, `#AC`, `#CP`).
- Save and restore the full CPU context during interrupts:
  - All general-purpose registers (`RAX` through `R15`, `RSP`, `RBP`, `RDI`, `RSI`, etc.) are pushed to the stack and preserved across the C handler call.
  - Stack is properly aligned and cleaned up (`add rsp, 8`) before executing `iretq` to gracefully return from interrupts.
- Define and generate the 64-bit Interrupt Gate structures within the macro:
  - The handler address is correctly split into three parts (low 16 bits, mid 16 bits, and high 32 bits).
  - The segment selector (`gdt64.code`), zero IST field, and correct interrupt-gate attributes (`0b10001110`) are properly set.
- Provide an external C-linkage symbol (`kernel_handler`) to connect the assembly stubs with a future exception-handling function written in C.

### Changed
- (None)

### Deprecated
- (None)

### Removed
- (None)

### Fixed
- (None)

### Security
- (None)

## [0.3.0] - 2026-06-26

### Added
- Complete implementation of Interrupt Descriptor Table (IDT) and exception handling (Roadmap Step 1 finished).
- New `init_idt()` function written in C:
  - Generates the 256-entry IDT using a loop and the `isr_stubs_point` array from assembly.
  - Correctly splits 64-bit handler addresses into the three required parts (low 16, mid 16, high 32 bits) using bitwise shifts.
  - Loads the IDT into the CPU using the `lidt` instruction.
- New `interrupts_handler()` function in C:
  - Successfully catches CPU exceptions (e.g., Divide Error `#DE`).
  - Prints "Opps..." to the VGA screen using the VGA driver.
  - Enters an infinite loop (`while(1)`) to halt the CPU and prevent a Triple Fault, confirming the handler works.
- Added `sti` (Set Interrupt Flag) instruction inside `kernel_main()` to enable hardware interrupts.
- New C file `kernel/interrupts.c` to manage the IDT and exception logic.

### Changed
- Refactored `isr.asm` generation: the NASM macro now generates **only** the 256 ISR stubs and a `idt_stubs_point` array of pointers, moving the actual IDT table generation to C.
- Updated `boot.asm` to call `init_idt()` during the boot sequence before jumping to the kernel entry points.

### Fixed
- Fixed the NASM assembly limitation where bitwise shift operators (`>>`) could not be applied to symbol addresses. Logic is now safely handled by the C compiler/linker during runtime initialization.
- Corrected context saving and stack alignment in the ISR stubs to properly handle error codes and return via `iretq`.

### Removed
- (None)

## [0.4.0] - 2026-06-30

### Added
- **Step 2 completed: Programmable Interrupt Controller (PIC) remapping and PIT timer (∼100 Hz).**
  - Implemented `ICW()` function to remap the PIC (Master: 0x20, Slave: 0x28) and disable all IRQs except IRQ0.
  - Initialized PIT channel 0 with a divider of 11931 to generate ~100 Hz interrupts.
  - Enabled IRQ0 by clearing bit 0 in the master interrupt mask (`outb(0x21, inb(0x21) & 0xFE)`).
  - Added `pit_handler()` which increments a tick counter and prints "Seconds elapsed: <hex>" every 100 ticks (≈1 second).
  - Properly sends End‑of‑Interrupt (EOI) to the master PIC in the IRQ0 handler.
- **New `stdlib.c` and `stdlib.h`** – added `long_to_hex()` function to convert unsigned long to a 16‑character hexadecimal string.
- **Expanded interrupt infrastructure:**
  - Introduced `dispatch_table[256]` to route interrupts to specific handlers (exceptions vs. hardware IRQs).
  - Added `exception_handler()` for CPU exceptions (vectors 0–31) that prints all registers and halts.
  - Wired `interrupts_handler()` to call the appropriate entry from `dispatch_table` based on the vector number.
- **Improved project organisation:**
  - Reordered declarations in `interrupts.c` to avoid incomplete type warnings and `undeclared identifier` errors.
  - Added `vga.h`, `stdlib.h`, and `ports.h` header files to centralise function prototypes.

### Changed
- **Fixed PIT frequency** – now correctly sends both the low and high bytes of the divider, resulting in a precise 100 Hz tick rate.
- Removed unnecessary `outb(0x20, 0x20)` (EOI) from `exception_handler` – it is only needed for hardware IRQs.
- Updated `boot.asm` to call `init_pit()` and `kernel_main_rs()` (Rust entry point) before enabling interrupts.

### Fixed
- Corrected the order of definitions in `interrupts.c` to satisfy C’s declaration‑before‑use rule.
- `pit_handler` now correctly resets `pit_ticks` after reaching 100, preventing overflow and ensuring consistent one‑second intervals.

### Removed
- (None)

### Security
- (None)

## [0.5.0] - 2026-07-01

### Added
- Modular interrupt subsystem architecture:
  - `idt.c` – IDT setup and loading.
  - `exceptions.c` – CPU exception handlers (vectors 0–31) with full register dump.
  - `pit.c` – PIT initialization and timer handler (100 Hz).
  - `pic.c` – PIC remapping (master 0x20, slave 0x28).
  - `interrupts.c` – central dispatcher (`interrupts_handler`) and `dispatch_table[256]` with `register_isr()` API.
- Common header `include/isr.h` defining `struct RegFrame`, `isr_handler_type`, and all public function prototypes.
- `init_interrupts()` as a single entry point for the whole interrupt subsystem.

### Changed
- Complete code reorganization: monolithic `interrupts.c` split into separate modules for IDT, exceptions, PIC/PIT, and dispatcher.
- Handler registration now uses `register_isr()` instead of direct assignment to `dispatch_table`.
- `ICW()` (PIC remapping) moved to a separate module and called from `init_pit()` (or separately).
- `boot.asm` now calls `init_interrupts()` instead of separate `init_idt()` and `init_pit()` calls.

### Removed
- (If `remap_pit.c` or old `interrupts.c` was deleted, mention it here)

### Fixed
- All compilation errors due to implicit function declarations (added prototypes in `isr.h`).
- Fixed `old-style function definition` and `implicit-int` warnings.