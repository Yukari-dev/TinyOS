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

# ADDED: -Ikernel allows you to #include "vga/vga.h" without relative paths
CFLAGS = -ffreestanding -m32 -fno-pic -fno-pie -fno-stack-protector -c -Ikernel
LDFLAGS = -m elf_i386 -T boot/linker.ld --oformat binary

# Directories
BOOT_DIR = boot
KERNEL_DIR = kernel
BUILD_DIR = build

# Files
BOOT_SRC = $(BOOT_DIR)/boot.asm
BOOT_BIN = $(BUILD_DIR)/boot.bin
DISK_IMG = myos.img

# --- AUTOMATIC FILE DETECTION ---
# Find all .c files in kernel/ and all its subdirectories (vga, stdio, etc.)
C_SOURCES = $(shell find $(KERNEL_DIR) -name "*.c")
# Convert those .c file paths to .o file paths in the build directory
# e.g., kernel/vga/vga.c becomes build/vga.o
C_OBJECTS = $(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

# Entry point (usually needs to be linked first)
KERNEL_ENTRY_ASM = $(KERNEL_DIR)/entry/kernelentry.asm
KERNEL_ENTRY_OBJ = $(BUILD_DIR)/kernelentry.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

# Default target
all: $(DISK_IMG)

# Create build directory and subfolders to mirror the kernel structure
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/vga
	mkdir -p $(BUILD_DIR)/stdio
	mkdir -p $(BUILD_DIR)/drivers
	mkdir -p $(BUILD_DIR)/cpu
	mkdir -p $(BUILD_DIR)/shell
	mkdir -p $(BUILD_DIR)/util

# Build bootloader
$(BOOT_BIN): $(BOOT_SRC) | $(BUILD_DIR)
	$(NASM) $(NASMFLAGS) $< -o $@

# AUTOMATIC RULE: Compile any .c file into a .o file
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c | $(BUILD_DIR)
	$(GCC) $(CFLAGS) $< -o $@

# Assemble kernel entry point
$(KERNEL_ENTRY_OBJ): $(KERNEL_ENTRY_ASM) | $(BUILD_DIR)
	$(NASM) $(KERNEL_NASMFLAGS) $< -o $@

# Link kernel (Entry object first, then all discovered C objects)
$(KERNEL_BIN): $(KERNEL_ENTRY_OBJ) $(C_OBJECTS) boot/linker.ld
	$(LD) $(LDFLAGS) $(KERNEL_ENTRY_OBJ) $(C_OBJECTS) -o $@

# Create disk image
$(DISK_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	$(DD) if=/dev/zero of=$@ bs=512 count=2880
	$(DD) if=$(BOOT_BIN) of=$@ bs=512 count=1 conv=notrunc
	$(DD) if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc

# Run in QEMU
run: $(DISK_IMG)
	$(QEMU) -drive format=raw,file=$< -display sdl

clean:
	rm -rf $(BUILD_DIR) $(DISK_IMG)

.PHONY: all run clean
