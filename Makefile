.PHONY: all clean showvars

SRCDIR = kernel
OBJDIR = obj

#Assebly
ASMSRCFILES = $(shell find $(SRCDIR) -type f -name "*.asm")
ASMSRCFILESNAMES = $(shell find $(SRCDIR) -type f -name "*.asm" -printf "%f\n")
ASMOBJFILESNAMES = $(patsubst %.asm,%.o,$(ASMSRCFILESNAMES))
ASMOBJFILES = $(patsubst %.asm,$(OBJDIR)/%.o,$(ASMSRCFILESNAMES))
ASSEMBLERFLAGS = -g -f elf32

#C
CSRCFILES = $(shell find $(SRCDIR) -type f -name "*.c")
CSRCFILESNAMES = $(shell find $(SRCDIR) -type f -name "*.c" -printf "%f\n")
COBJFILESNAMES = $(patsubst %.c,%.o,$(CSRCFILESNAMES))
COBJFILES = $(patsubst %.c,$(OBJDIR)/%.o,$(CSRCFILESNAMES))

# Binutils and cross-compiler
UTILSPATH = ~/opt/cross-compilers/bare-bones-default/bin
ASSEMBLER = nasm
CC = i686-elf-gcc
WARNINGS = -Werror -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
CFLAGS = -g3 -std=gnu99 -ffreestanding -I$(LIBCINCLUDEDIR) $(WARNINGS)

#Linker
#Linking result
KERNELBINFILENAME = kernel-0.bin
LINKER = i686-elf-gcc
LINKERSCRIPTFILE = linker.ld
LINKERFLAGS = -ffreestanding -nostdlib -lgcc

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

$(KERNELBINFILENAME): $(OBJDIR) $(ASMOBJFILES) $(COBJFILES) $(LIBCKTARGET)
	$(UTILSPATH)/$(LINKER) -T $(LINKERSCRIPTFILE) -o $(KERNELBINFILENAME) $(ASMOBJFILES) $(COBJFILES) $(LIBCKTARGET) $(LINKERFLAGS)
	@if ! grub-file --is-x86-multiboot $(KERNELBINFILENAME); then \
        echo $(KERNELBINFILENAME) is not a multiboot! ; \
		echo Remove $(KERNELBINFILENAME) ; \
		rm $(KERNELBINFILENAME) ; \
		exit 1 ; \
    fi

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/start.o: $(SRCDIR)/start.asm
	$(ASSEMBLER) $(ASSEMBLERFLAGS) $(SRCDIR)/start.asm -o $@

$(OBJDIR)/kmain.o: $(SRCDIR)/kmain.c $(SRCDIR)/egatextmode/egatextmode.h
	$(UTILSPATH)/$(CC) -c $(SRCDIR)/kmain.c -o $@ $(CFLAGS)

$(OBJDIR)/egatextmode.o: $(SRCDIR)/egatextmode/egatextmode.c $(SRCDIR)/egatextmode/egatextmode.h
	$(UTILSPATH)/$(CC) -c $(SRCDIR)/egatextmode/egatextmode.c -o $@ $(CFLAGS)

clean:
	rm -r -f $(OBJDIR)

showvars:
	@echo $(ASMSRCFILES)
	@echo $(ASMOBJFILES)
	@echo $(ASMOBJFILESNAMES)
	@echo $(CSRCFILES)
	@echo $(COBJFILES)
	@echo $(COBJFILESNAMES)
