.PHONY: all runqemu clean showvars

BUILD_DIR = ./build
OBJS_DIR = $(BUILD_DIR)/objs
SRCS_DIR = .

BOOTABLE_ISO_NAME = bootablekernel.iso
BOOTABLE_ISO_FILE = $(BUILD_DIR)/$(BOOTABLE_ISO_NAME)

# C compiler settings
GCC = i686-elf-gcc
GCC_WARNINGS = -Werror -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
LIBC_INCLUDE = $(SRCS_DIR)/libc/include
GCC_FLAGS = -O0 -g3 -std=gnu11 -ffreestanding -masm=intel -I$(LIBC_INCLUDE) $(GCC_WARNINGS)

# Assembler settings
NASM = nasm
NASM_FLAGS = -g -f elf32 -w+error=all

# Linker settings
KERNEL_BIN_NAME = kernel-0.bin
KERNEL_BIN_FILE = $(BUILD_DIR)/$(KERNEL_BIN_NAME)
LINKER = i686-elf-gcc
LINKER_SCRIPT_FILE = $(SRCS_DIR)/kernel/arch/i386/linker.ld
LINKER_FLAGS = -ffreestanding -nostdlib -lgcc -Wl,-Map=$(SRCS_DIR)/kernel.map
#print memory map
#-Wl,-Map=kernel.map

# GRUB
# must contain boot/grub/grub.cfg
ISO_DIR = $(SRCS_DIR)/iso

# C sources
SRCS_C_FILES = $(shell find $(SRCS_DIR) -name '*.c' -or -name '*.cpp')
# just the names of .c files, without paths
# ./kernel/arch/i386/gdt/gdt.c -> gdt.c
#SRCS_C_FILES_NAMES = $(notdir $(SRCS_C_FILES))
SRCS_H_FILES = $(shell find $(SRCS_DIR) -name '*.h')

# C objects
# just the names of .o files, without paths
# ./kernel/arch/i386/gdt/gdt.c -> gdt.o
#OBJS_C_FILES_NAMES = $(patsubst %.c, %.o, $(SRCS_C_FILES_NAMES))
# gdt.o -> $(OBJS_DIR)/gdt.o
#OBJS_C_FILES = $(addprefix $(OBJS_DIR)/, $(OBJS_C_FILES_NAMES))

# ./kernel/arch/i386/gdt/gdt.c -> ./kernel/arch/i386/gdt/gdt.o
OBJS_C_FILES = $(patsubst %.c, %.o, $(SRCS_C_FILES))
# ./kernel/arch/i386/gdt/gdt.o -> kernel/arch/i386/gdt/gdt.o
OBJS_C_FILES := $(patsubst ./%, %, $(OBJS_C_FILES))
# kernel/arch/i386/gdt/gdt.o -> $(OBJS_DIR)/kernel/arch/i386/gdt/gdt.o
OBJS_C_FILES := $(addprefix $(OBJS_DIR)/, $(OBJS_C_FILES))

# ./kernel/arch/i386/gdt/gdt.c -> ./build/objs/./kernel/arch/i386/gdt/gdt.o
#OBJS_C_FILES = $(addprefix $(OBJS_DIR)/, $(patsubst %.c, %.o, $(SRCS_C_FILES)))
# We need to create folders for objects before we can start compiling objects
# ./build/objs/./kernel/arch/i386/gdt/gdt.o -> ./build/objs/./kernel/arch/i386/gdt/
#OBJS_C_FILES_DIRS = $(dir $(OBJS_C_FILES))

# Assembly sources
SRCS_ASM_FILES = $(shell find $(SRCS_DIR) -name '*.asm')
# ./kernel/arch/i386/start.asm -> start.asm
#SRCS_ASM_FILES_NAMES = $(notdir $(SRCS_ASM_FILES))

# Assembly objects
# start.asm -> start.o
# OBJS_ASM_FILES_NAMES = $(patsubst %.asm, %.o, $(SRCS_ASM_FILES_NAMES))
#OBJS_ASM_FILES_NAMES_WITHOUT_CRT_OBJS_FOR_LINKER = $(filter-out $(CRTI_O) $(CRTN_O),$(OBJS_ASM_FILES_NAMES))
# start.o -> $(OBJS_DIR)/start.o
#OBJS_ASM_FILES = $(addprefix $(OBJS_DIR)/, $(OBJS_ASM_FILES_NAMES))
#OBJS_ASM_FILES_WITHOUT_CRT_OBJS_FOR_LINKER = $(addprefix $(OBJS_DIR)/, $(OBJS_ASM_FILES_NAMES_WITHOUT_CRT_OBJS_FOR_LINKER))

# ./kernel/arch/i386/interrupts/isrs.asm -> ./kernel/arch/i386/interrupts/isrs.o
OBJS_ASM_FILES := $(patsubst %.asm, %.o, $(SRCS_ASM_FILES))
# ./kernel/arch/i386/interrupts/isrs.o -> kernel/arch/i386/interrupts/isrs.o
OBJS_ASM_FILES := $(patsubst ./%, %, $(OBJS_ASM_FILES))
# kernel/arch/i386/interrupts/isrs.o -> $(OBJS_DIR)/kernel/arch/i386/interrupts/isrs.o
OBJS_ASM_FILES := $(addprefix $(OBJS_DIR)/, $(OBJS_ASM_FILES))

# We need to remove crti.o and crtn.o from asm object list, because they must be passed in the correct order during linking
CRTI_O_NAME = crti.o
CRTN_O_NAME = crtn.o
CRTI_O_FILE = $(filter %/$(CRTI_O_NAME), $(OBJS_ASM_FILES))
CRTN_O_FILE = $(filter %/$(CRTN_O_NAME), $(OBJS_ASM_FILES))
CRTBEGIN_O_FILE = $(shell $(GCC) $(GCC_FLAGS) -print-file-name=crtbegin.o)
CRTEND_O_FILE = $(shell $(GCC) $(GCC_FLAGS) -print-file-name=crtend.o)
CRT_FILES = $(CRTI_O_FILE) $(CRTBEGIN_O_FILE) $(CRTEND_O_FILE) $(CRTN_O_FILE)
OBJS_ASM_FILES_WITHOUT_CRTIN_OBJS_FOR_LINKER = $(filter-out %/$(CRTI_O_NAME) %/$(CRTN_O_NAME), $(OBJS_ASM_FILES))

# Creates the necessary folder for the file, if it does not already exist
objs_dir_guard=$(shell [ ! -d $(@D) ] && mkdir -p $(@D))

all: $(BOOTABLE_ISO_FILE)
	@cp $(KERNEL_BIN_FILE) $(SRCS_DIR)
	@cp $(BOOTABLE_ISO_FILE) $(SRCS_DIR)

$(BOOTABLE_ISO_FILE): $(ISO_DIR)/boot/grub/grub.cfg $(KERNEL_BIN_FILE) 
	@echo [BUILDING iso] $@
	@rm -f $@
	@cp $(KERNEL_BIN_FILE) $(ISO_DIR)/boot/$(KERNEL_BIN_NAME)
	@grub-mkrescue -o $(BOOTABLE_ISO_FILE) $(ISO_DIR)

$(ISO_DIR)/boot/grub/grub.cfg:
	@echo "[ERROR cfg]: $@ NOT EXIST!"

$(KERNEL_BIN_FILE): $(LINKER_SCRIPT_FILE) $(OBJS_C_FILES) $(OBJS_ASM_FILES) $(CRT_FILES)
	@echo "[BUILDING linking] $@"
	@rm -f $@
	@$(objs_dir_guard)
	@$(LINKER) -T $(LINKER_SCRIPT_FILE) -o $@ \
								$(CRTI_O_FILE) $(CRTBEGIN_O_FILE) \
								$(OBJS_ASM_FILES_WITHOUT_CRTIN_OBJS_FOR_LINKER) \
								$(OBJS_C_FILES) \
								$(CRTEND_O_FILE) $(CRTN_O_FILE) \
								$(LINKER_FLAGS)
	@if ! grub-file --is-x86-multiboot2 $@; then \
        echo [ERROR linking] $@ is not a multiboot! ; \
		exit 1 ; \
    fi


$(LINKER_SCRIPT_FILE):
	@echo "[ERROR ld]: $@ NOT EXIST!"

# Compile C
$(OBJS_DIR)/%.o: %.c $(SRCS_H_FILES)
	@echo "[BUILDING c] $@"
	@rm -f $@
	@$(objs_dir_guard)
	@$(GCC) -c $< -o $@ $(GCC_FLAGS)

# Assembly asm
$(OBJS_DIR)/%.o: %.asm
	@echo "[BUILDING asm] $@"
	@rm -f $@
	@$(objs_dir_guard)
	@$(NASM) $(NASM_FLAGS) $< -o $@

# QEMU
QEMU = qemu-system-i386
#QEMUFLAGS = -d int -no-shutdown -no-reboot -m 128M -s -S -monitor stdio
QEMUFLAGS = -no-shutdown -no-reboot -m 32M -s -S -monitor stdio -serial file:serial.log

runqemu:
	$(QEMU) $(QEMUFLAGS) $(BOOTABLE_ISO_NAME)

clean:
	rm -f $(KERNEL_BIN_NAME)
	rm -f $(BOOTABLE_ISO_NAME)
	rm -f $(ISO_DIR)/boot/$(KERNEL_BIN_NAME)
	rm -rf $(BUILD_DIR)

showvars:
#	@echo $(SRCS_C_FILES)
#	@echo $(SRCS_ASM_FILES)
#	@echo --------
#	@echo $(OBJS_C_FILES)
#	@echo $(OBJS_ASM_FILES)
