# Makefile for TinyOS

# Tools
NASM = nasm
GCC = gcc
LD = ld
DD = dd
QEMU = qemu-system-x86_64

# Flags
NASMFLAGS = -f bin
KERNEL_NASMFLAGS = -f elf32
CFLAGS = -ffreestanding -m32 -fno-pic -fno-pie -c
LDFLAGS = -m elf_i386 -T boot/linker.ld --oformat binary

# Directories
BOOT_DIR = boot
KERNEL_DIR = kernel
BUILD_DIR = build

# Files
BOOT_SRC = $(BOOT_DIR)/boot.asm
BOOT_BIN = $(BUILD_DIR)/boot.bin

KERNEL_C = $(KERNEL_DIR)/kernel.c
KERNEL_ENTRY_ASM = $(KERNEL_DIR)/kernelentry.asm

KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KERNEL_ENTRY_OBJ = $(BUILD_DIR)/kernelentry.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

DISK_IMG = myos.img

# Default target
all: $(DISK_IMG)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build bootloader
$(BOOT_BIN): $(BOOT_SRC) | $(BUILD_DIR)
	$(NASM) $(NASMFLAGS) $< -o $@

# Compile kernel C code
$(KERNEL_OBJ): $(KERNEL_C) | $(BUILD_DIR)
	$(GCC) $(CFLAGS) $< -o $@

# Assemble kernel entry point
$(KERNEL_ENTRY_OBJ): $(KERNEL_ENTRY_ASM) | $(BUILD_DIR)
	$(NASM) $(KERNEL_NASMFLAGS) $< -o $@

# Link kernel
$(KERNEL_BIN): $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) boot/linker.ld
	$(LD) $(LDFLAGS) $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) -o $@

# Create floppy disk image
$(DISK_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	$(DD) if=/dev/zero of=$@ bs=512 count=2880
	$(DD) if=$(BOOT_BIN) of=$@ bs=512 count=1 conv=notrunc
	$(DD) if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc

# Run in QEMU
run: $(DISK_IMG)
	$(QEMU) -drive format=raw,file=$< -display sdl

# Debug with QEMU monitor
debug: $(DISK_IMG)
	$(QEMU) -fda $< -monitor stdio

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(DISK_IMG)

# Phony targets
.PHONY: all run debug clean
