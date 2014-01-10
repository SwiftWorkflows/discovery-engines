
# User may have to set these to correct locations:
WOZNIAK = /homes/wozniak/Public
# MCS Public:
MPI_HOME = $(WOZNIAK)/mpich-master
SWIG = $(WOZNIAK)/swig-2.0.4-x86_64/bin/swig
TCL_HOME = $(WOZNIAK)/tcl-8.6.1-x86_64
# Wozniak's laptop:
# MPI_HOME = $(HOME)/sfw/mpich-master
# SWIG = $(HOME)/sfw/swig-2.0.4/bin/swig
# TCL_HOME = $(HOME)/sfw/tcl-8.6.0

OBJS = user_wrap.o user.o

MPICC = $(MPI_HOME)/bin/mpicc
TCLSH = $(wildcard $(TCL_HOME)/bin/tclsh8.* )

MPI_INCLUDE = $(MPI_HOME)/include
TCL_INCLUDE = $(TCL_HOME)/include

INCLUDES :=
INCLUDES += -I $(MPI_INCLUDE)
INCLUDES += -I $(TCL_INCLUDE)

CFLAGS += -g -O0 -fPIC -Wall
CFLAGS += $(INCLUDES)

all: pkgIndex.tcl

user_wrap.c: user.h
	$(SWIG) -module user $(<)

libuser.so: $(OBJS)
	mpicc -shared -o $(@) $(OBJS)

pkgIndex.tcl: libuser.so make-package.tcl
	LEAF_PKG=user LEAF_VERSION=0.0  \
	LEAF_SO=$(<) LEAF_TCL=user.tcl \
	$(TCLSH) make-package.tcl > $(@)

clean:
	@rm -fv *.o *.so *_wrap.c pkgIndex.tcl
