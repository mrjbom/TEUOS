.PHONY: error libcclean libcshowvars

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
$(LIBCOBJDIR)/strlen.o \
$(LIBCOBJDIR)/memset.o \
$(LIBCOBJDIR)/memcpy.o \
$(LIBCOBJDIR)/memcmp.o \
$(LIBCOBJDIR)/memmove.o

LIBCHOSTEDOBJFILES =\
#$(LIBCOBJDIR)/malloc.o #for example

LIBCCOBJS =\
$(LIBCFREEOBJFILES) \
$(LIBCHOSTEDOBJFILES)

LIBCKOBJS = $(LIBCFREEOBJFILES:.o=.libk.o)

$(LIBCDIR)/libk.a: $(LIBCOBJDIR) $(LIBCKOBJS)
	$(UTILSPATH)/$(LINKER) -r -o $@ $(LIBCKOBJS) $(LINKERFLAGS)

$(LIBCOBJDIR)/strlen.libk.o: $(LIBCDIR)/string/strlen.c $(LIBCINCLUDEDIR)/string.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(LIBCKCFLAGS)

$(LIBCOBJDIR)/memset.libk.o: $(LIBCDIR)/string/memset.c $(LIBCINCLUDEDIR)/string.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(LIBCKCFLAGS)

$(LIBCOBJDIR)/memcpy.libk.o: $(LIBCDIR)/string/memcpy.c $(LIBCINCLUDEDIR)/string.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(LIBCKCFLAGS)

$(LIBCOBJDIR)/memcmp.libk.o: $(LIBCDIR)/string/memcmp.c $(LIBCINCLUDEDIR)/string.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(LIBCKCFLAGS)

$(LIBCOBJDIR)/memmove.libk.o: $(LIBCDIR)/string/memmove.c $(LIBCINCLUDEDIR)/string.h
	$(UTILSPATH)/$(CC) -c $< -o $@ $(LIBCKCFLAGS)

$(LIBCOBJDIR):
	mkdir -p $(LIBCOBJDIR)

libcclean:
	rm -r -f $(LIBCOBJDIR)
	rm -r -f $(LIBCKTARGET)

libcshowvars:
	@echo $(LIBCSRCFILES)
	@echo $(LIBCSRCFILESNAMES)
	@echo '"$(LIBCCOBJS)"'
	@echo '"$(LIBCKOBJS)"'