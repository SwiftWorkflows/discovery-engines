
all: bin/sro

# Compilers
# Linking with the h5.. compilers does not work for Python
CC = h5cc
FC = h5fc
CLINKER = gcc
FLINKER = gfortran

# Flags common to Fortran and C
INCLUDES = -I . -I src -I src/c
FLAGS := -Wall -g -fPIC
ifeq ($(FAST),1)
	FLAGS += -O3
else
	FLAGS += -O0
endif
ifeq (py,$(findstring py,$(MAKECMDGOALS)))
# 	FLAGS += -fPIC
	INCLUDES += -I /usr/include/python2.7
endif
FLAGS += $(INCLUDES)

FFLAGS = $(FLAGS) -fimplicit-none -Wuninitialized -pedantic -cpp -J mods
CFLAGS = $(FLAGS) $(INCLUDES) -std=gnu99

# Linking with the h5.. compilers does not work for Python
HDF_LIBS = -L /usr/lib/x86_64-linux-gnu/hdf5/serial -l hdf5_fortran \
		-Wl,-rpath -Wl,/usr/lib/x86_64-linux-gnu/hdf5/serial

LIBPY = lib/_libpysro.so

py: $(LIBPY)

include src/module.mk
include bin/module.mk
include lib/module.mk
include mods/module.mk
include py/module.mk
include tests/module.mk

clean::
	@echo CLEAN

.PRECIOUS: %.o
