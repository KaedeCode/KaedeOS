# Development Roadmap

Below is a step-by-step plan for building the operating system kernel, broken down by task, language, and status.

| # | Task | Language | Status |
|---|---|---|---|
| 0 | Setup tools, Makefile, project structure | – | ◼ |
| 1 | PE header, UEFI entry point, system table calls | NASM | ◻ |
| 2 | Get GOP, set video mode, draw pixels | C | ◻ |
| 3 | FAT32 driver, read kernel file from disk | C + NASM | ◻ |
| 4 | Jump to kernel, pass parameters | NASM + C | ◻ |
| 5 | Kernel: initialization, framebuffer output (font) | C++ | ◻ |
| 6 | Kernel: IDT and simple keyboard handler | C++ + NASM | ◻ |
| 7 | (Optional) Rewrite graphics in Rust | Rust | ◻ |
| 8 | Memory management (paging) | C++ | ◻ |
| 9 | Load ELF files and create processes | C++ / Rust | ◻ |
| 10 | PS/2 keyboard driver | Rust | ◻ |