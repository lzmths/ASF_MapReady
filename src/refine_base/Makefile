CFLAGS += $(HDF5_CFLAGS)
CFLAGS += $(GEOTIFF_CFLAGS)
CFLAGS += $(HDF5_CFLAGS)
# Makefile for		: refine_base
# Module Author		: Rob Fatland & Mike Shindle

include ../../make_support/system_rules

LIBS = \
	$(LIBDIR)/libasf_insar.a \
	$(LIBDIR)/libifm.a \
	$(LIBDIR)/asf_meta.a \
	$(GSL_LIBS) \
	$(LIBDIR)/libasf_proj.a \
	$(PROJ_LIBS) \
	$(LIBDIR)/asf.a \
	$(XML_LIBS) \
	-lm

OBJS = 	getphase.o \
	genab.o \
	bp.o \
	test_base.o \
	refine_base.o

all: refine_base

refine_base: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o refine_base $(OBJS) $(LIBS) $(LDFLAGS)
	mv refine_base$(BIN_POSTFIX) $(BINDIR)
	rm *.o
	cp refine_base.1 ../../man/man1
