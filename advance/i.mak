############################################################################
# I

ICFLAGS += \
	-I$(srcdir)/advance/lib
IOBJDIRS += \
	$(IOBJ) \
	$(IOBJ)/i \
	$(IOBJ)/lib
IOBJS += \
	$(IOBJ)/i/i.o \
	$(IOBJ)/lib/log.o \
	$(IOBJ)/lib/conf.o \
	$(IOBJ)/lib/incstr.o \
	$(IOBJ)/lib/device.o \
	$(IOBJ)/lib/inputdrv.o \
	$(IOBJ)/lib/inputall.o \
	$(IOBJ)/lib/inone.o \
	$(IOBJ)/lib/error.o

ifeq ($(CONF_HOST),unix)
ICFLAGS += \
	-DPREFIX=\"$(PREFIX)\" \
	-I$(srcdir)/advance/linux \
	-DUSE_INPUT_NONE
IOBJDIRS += \
	$(IOBJ)/linux
IOBJS += \
	$(IOBJ)/lib/filenix.o \
	$(IOBJ)/lib/targnix.o \
	$(IOBJ)/linux/os.o
ifeq ($(CONF_LIB_SLANG),yes)
ICFLAGS += \
	-DUSE_INPUT_SLANG 
ILIBS += -lslang
IOBJS += \
	$(IOBJ)/linux/islang.o
endif
endif

ifeq ($(CONF_HOST),dos)
ICFLAGS += \
	-I$(srcdir)/advance/dos \
	-DUSE_INPUT_DOS -DUSE_INPUT_NONE
ILIBS += -lalleg
IOBJDIRS += \
	$(IOBJ)/dos
IOBJS += \
	$(IOBJ)/lib/filedos.o \
	$(IOBJ)/lib/targdos.o \
	$(IOBJ)/dos/os.o \
	$(IOBJ)/dos/idos.o
endif

$(IOBJ)/%.o: $(srcdir)/advance/%.c
	$(ECHO) $@ $(MSG)
	$(CC) $(CFLAGS) $(ICFLAGS) -c $< -o $@

$(sort $(IOBJDIRS)):
	$(ECHO) $@
	$(MD) $@

$(IOBJ)/advi$(EXE) : $(sort $(IOBJDIRS)) $(IOBJS)
	$(ECHO) $@ $(MSG)
	$(LD) $(LDFLAGS) $(ILDFLAGS) $(IOBJS) $(ILIBS) -o $@
ifeq ($(CONF_COMPRESS),yes)
	$(UPX) $@
	$(TOUCH) $@
endif
	$(RM) advi$(EXE)
	$(LN_S) $@ advi$(EXE)

