.PHONY: all clean showvars
.DEFAULT_GOAL := all

SRCDIR = kernel
OBJDIR = obj
ARCHI386DIR = kernel/arch/i386

#Assebly
ASMSRCFILES = $(shell find $(SRCDIR) -type f -name "*.asm")
ASMSRCFILESNAMES = $(shell find $(SRCDIR) -type f -name "*.asm" -printf "%f\n")
ASMOBJFILESNAMES = $(patsubst %.asm,%.o,$(ASMSRCFILESNAMES))
ASMOBJFILES = $(patsubst %.asm,$(OBJDIR)/%.o,$(ASMSRCFILESNAMES))
ASSEMBLER = nasm
ASSEMBLERFLAGS = -g -f elf32

#C
CSRCFILES = $(shell find $(SRCDIR) -type f -name "*.c")
CSRCFILESNAMES = $(shell find $(SRCDIR) -type f -name "*.c" -printf "%f\n")
COBJFILESNAMES = $(patsubst %.c,%.o,$(CSRCFILESNAMES))
COBJFILES = $(patsubst %.c,$(OBJDIR)/%.o,$(CSRCFILESNAMES))

#Binutils and cross-compiler
UTILSPATH = ~/opt/cross-compilers/bare-bones-default/bin
CC = i686-elf-gcc
WARNINGS = -Werror -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
CFLAGS = -O0 -g3 -std=gnu99 -ffreestanding -masm=intel -I$(LIBCINCLUDEDIR) $(WARNINGS)

#Linker
#Linking result
KERNELBINFILENAME = kernel-0.bin
LINKER = i686-elf-gcc
LINKERSCRIPTFILE = $(ARCHI386DIR)/linker.ld
LINKERFLAGS = -ffreestanding -nostdlib -lgcc -Wl,-Map=kernel.map
#print memory map
#-Wl,-Map=kernel.map

CRTIOBJ = $(OBJDIR)/crti.o
CRTBEGINOBJ = $(shell $(UTILSPATH)/$(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTENDOBJ = $(shell $(UTILSPATH)/$(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTNOBJ = $(OBJDIR)/crtn.o
CRTOBJFILES = $(CRTIOBJ) $(CRTBEGINOBJ) $(CRTENDOBJ) $(CRTNOBJ)

ASMOBJLINKFILES = $(filter-out $(CRTOBJFILES),$(ASMOBJFILES))

#GRUB
BOOTABLEISOFILENAME = bootablekernel.iso
#must contain boot/grub/grub.cfg
ISODIR = iso

include libc/Makefile

all: $(LIBCTARGETS) $(BOOTABLEISOFILENAME)

$(BOOTABLEISOFILENAME): $(ISODIR)/boot/grub/grub.cfg $(KERNELBINFILENAME)
	cp $(KERNELBINFILENAME) $(ISODIR)/boot/$(KERNELBINFILENAME)
	grub-mkrescue -o $(BOOTABLEISOFILENAME) $(ISODIR)

$(ISODIR)/boot/grub/grub.cfg:
	mkdir -p $(ISODIR)/boot/grub
	touch $(ISODIR)/boot/grub/grub.cfg

$(KERNELBINFILENAME): $(OBJDIR) $(LINKERSCRIPTFILE) $(ASMOBJFILES) $(COBJFILES) $(LIBCKTARGET)
	$(UTILSPATH)/$(LINKER) -T $(LINKERSCRIPTFILE) -o $(KERNELBINFILENAME) \
	$(CRTIOBJ) $(CRTBEGINOBJ) \
	$(ASMOBJLINKFILES) \
	$(COBJFILES) \
	$(LIBCKTARGET) \
	$(CRTENDOBJ) $(CRTNOBJ) \
	$(LINKERFLAGS)
	@if ! grub-file --is-x86-multiboot $(KERNELBINFILENAME); then \
        echo $(KERNELBINFILENAME) is not a multiboot! ; \
		echo Remove $(KERNELBINFILENAME) ; \
		rm $(KERNELBINFILENAME) ; \
		exit 1 ; \
    fi

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/start.o: $(ARCHI386DIR)/start.asm
	$(ASSEMBLER) $(ASSEMBLERFLAGS) $< -o $@

$(OBJDIR)/kmain.o: $(SRCDIR)/kmain.c $(ARCHI386DIR)/egatextmode/egatextmode.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/egatextmode.o: $(ARCHI386DIR)/egatextmode/egatextmode.c $(ARCHI386DIR)/egatextmode/egatextmode.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/mmu.o: $(ARCHI386DIR)/mmu/mmu.c $(ARCHI386DIR)/mmu/mmu.h $(ARCHI386DIR)/mmu/pde.h $(ARCHI386DIR)/mmu/pte.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/pde.o: $(ARCHI386DIR)/mmu/pde.c $(ARCHI386DIR)/mmu/pde.h $(LIBCINCLUDEDIR)/bits.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/pte.o: $(ARCHI386DIR)/mmu/pte.c $(ARCHI386DIR)/mmu/pte.h $(LIBCINCLUDEDIR)/bits.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/gdt.o: $(ARCHI386DIR)/gdt/gdt.c $(ARCHI386DIR)/gdt/gdt.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/gdt_flush.o: $(ARCHI386DIR)/gdt/gdt_flush.asm
	$(ASSEMBLER) $(ASSEMBLERFLAGS) $< -o $@

$(OBJDIR)/interrupts.o: $(ARCHI386DIR)/interrupts/interrupts.c $(ARCHI386DIR)/interrupts/interrupts.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/exceptions.o: $(ARCHI386DIR)/interrupts/exceptions/exceptions.c $(ARCHI386DIR)/interrupts/exceptions/exceptions.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/idt.o: $(ARCHI386DIR)/interrupts/idt/idt.c $(ARCHI386DIR)/interrupts/idt/idt.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/idt_flush.o: $(ARCHI386DIR)/interrupts/idt/idt_flush.asm
	$(ASSEMBLER) $(ASSEMBLERFLAGS) $< -o $@

$(OBJDIR)/isrs.o: $(ARCHI386DIR)/interrupts/isrs.asm
	$(ASSEMBLER) $(ASSEMBLERFLAGS) $< -o $@

$(OBJDIR)/isr_handler.o: $(ARCHI386DIR)/interrupts/isr_handler.c $(ARCHI386DIR)/interrupts/isr_handler.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(CFLAGS)

$(CRTIOBJ): $(ARCHI386DIR)/crti.asm
	$(ASSEMBLER) $(ASSEMBLERFLAGS) $< -o $@

$(CRTNOBJ): $(ARCHI386DIR)/crtn.asm
	$(ASSEMBLER) $(ASSEMBLERFLAGS) $< -o $@

clean: libcclean
	rm -r -f $(OBJDIR)
	rm -r -f $(KERNELBINFILENAME)
	rm -r -f $(BOOTABLEISOFILENAME)

showvars:
	@echo $(ASMSRCFILES)
	@echo $(ASMOBJFILES)
	@echo $(ASMOBJLINKFILES)
	@echo $(CSRCFILES)
	@echo $(COBJFILES)

QEMUFLAGS = -d int -no-reboot -m 128M -s -S -monitor stdio

runqemu:
	qemu-system-i386 $(QEMUFLAGS) $(BOOTABLEISOFILENAME)
