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