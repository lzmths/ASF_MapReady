CFLAGS += $(HDF5_CFLAGS)
CFLAGS += $(GEOTIFF_CFLAGS)
# Makefile for          :  airsarin 
# Module Author         :  Joshua Badgley 

include ../../make_support/system_rules

CFLAGS += $(HDF5_CFLAGS)

LIBS =  $(LIBDIR)/asf_meta.a \
	$(LIBDIR)/libasf_proj.a \
	$(LIBDIR)/asf.a \
	$(XML_LIBS) \
	$(GSL_LIBS) \
	$(PROJ_LIBS) \
	-lm

OBJS = airsarin.o \
       airsar2ddr.o \
       airsar_io.o

airsarin: $(OBJS)
	$(CC) $(CFLAGS) -o airsarin $(OBJS) $(LIBS)
	mv airsarin$(BIN_POSTFIX) $(BINDIR)
	cp airsarin.1 ../../man/cat1/airsarin.1
	rm -f $(OBJS)

