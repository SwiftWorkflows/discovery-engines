
CFLAGS += -std=gnu99 -g

HDF = /usr
# HDF = $(HOME)/sfw/hdf5-1.8.11
CFLAGS += -I $(HDF)/include

BINS :=

include src/mt2/module.mk

all: $(BINS)

clean::
	rm -fv $(BINS)

