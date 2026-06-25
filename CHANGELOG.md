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