ASM=nasm
CC=x86_64-elf-gcc
CXX=x86_64-elf-g++
RUSTC=rustc
LD=x86_64-elf-gcc

CFLAGS=-ffreestanding -Wall -Wextra -O2 -m64 -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -fno-stack-protector -fno-pic -fno-pie -Iinclude
CXXFLAGS=$(CFLAGS) -fno-rtti -fno-exceptions
RUSTFLAGS=--target x86_64-unknown-none -C panic=abort -C opt-level=2 -C relocation-model=static -C no-redzone -C code-model=large
LDFLAGS=-T linker.ld -ffreestanding -nostdlib -lgcc -static -no-pie -z max-page-size=0x1000

BUILD_DIR=build
ISO_DIR=iso
KERNEL_ELF=$(ISO_DIR)/boot/kernel.elf
GRUB_CFG=$(ISO_DIR)/boot/grub/grub.cfg
ISO=kaedeos.iso

ASM_SOURCES=$(shell find bootloader kernel -name '*.asm')
C_SOURCES=$(shell find kernel libc drivers utils -name '*.c')
CPP_SOURCES=$(shell find kernel -name '*.cpp')
RS_SOURCES=$(shell find kernel -name '*.rs')

ASM_OBJS=$(patsubst %.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
C_OBJS=$(patsubst %.c,$(BUILD_DIR)/%_c.o,$(C_SOURCES))
CPP_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%_cpp.o,$(CPP_SOURCES))
RS_OBJS=$(patsubst %.rs,$(BUILD_DIR)/%_rs.o,$(RS_SOURCES))
OBJS=$(ASM_OBJS) $(C_OBJS) $(CPP_OBJS) $(RS_OBJS)

all: $(ISO)

$(ISO): $(KERNEL_ELF) $(GRUB_CFG)
	grub-mkrescue -o $(ISO) $(ISO_DIR)

$(KERNEL_ELF): $(OBJS)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) -f elf64 $< -o $@

$(BUILD_DIR)/%_c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%_cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%_rs.o: %.rs
	@mkdir -p $(dir $@)
	$(RUSTC) $(RUSTFLAGS) --emit obj $< -o $@

$(GRUB_CFG):
	@mkdir -p $(dir $@)
	echo "set timeout=5" > $@
	echo "set default=0" >> $@
	echo "" >> $@
	echo "menuentry \"KaedeOS\" {" >> $@
	echo "  multiboot2 /boot/kernel.elf" >> $@
	echo "  boot" >> $@
	echo "}" >> $@

run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO) -m 512M -smp 2

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(ISO)

.PHONY: all run clean