.PHONY: error buildlibc libcclean libcshowvars

#LIBCC - libc user-space part
#LIBCK - libc(libk) kernel part

LIBCDIR = libc
LIBCINCLUDEDIR = $(LIBCDIR)/include
LIBCOBJDIR = $(LIBCDIR)/libcobj

LIBCKCFLAGS = $(CFLAGS) -D__is_libk

LIBCKTARGET = $(LIBCDIR)/libk.a
LIBCCTARGET = $(LIBCDIR)/libc.a
LIBCTARGETS = $(LIBCKTARGET) #$(LIBCCTARGET)

LIBCSRCFILES = $(shell find $(LIBCDIR) -type f -name "*.c")
LIBCSRCFILESNAMES = $(shell find $(LIBCDIR) -type f -name "*.c" -printf "%f\n")

LIBCFREEOBJFILES =\
$(LIBCOBJDIR)/strlen.o

LIBCHOSTEDOBJFILES =\
#$(LIBCOBJDIR)/malloc.o #for example

LIBCCOBJS =\
$(LIBCFREEOBJFILES) \
$(LIBCHOSTEDOBJFILES)

LIBCKOBJS = $(LIBCFREEOBJFILES:.o=.libk.o)

error:
	$(error use "make all")

$(LIBCDIR)/libk.a: $(LIBCOBJDIR) $(LIBCKOBJS)
	$(UTILSPATH)/$(LINKER) -r -o $@ $(LIBCKOBJS) $(LINKERFLAGS)

$(LIBCOBJDIR)/strlen.libk.o: $(LIBCDIR)/string/strlen.c $(LIBCINCLUDEDIR)/string.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(LIBCKCFLAGS)

$(LIBCOBJDIR):
	mkdir -p $(LIBCOBJDIR)

libcclean:
	rm -r -f $(LIBCOBJDIR)

libcshowvars:
	@echo $(LIBCSRCFILES)
	@echo $(LIBCSRCFILESNAMES)
	@echo '"$(LIBCCOBJS)"'
	@echo '"$(LIBCKOBJS)"'