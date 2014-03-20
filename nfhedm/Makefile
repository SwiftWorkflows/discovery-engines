
# Set software locations here:
NLOPT = $(HOME)/sfw/nlopt-2.4.1
# NLOPT = $(HOME)/sfw/nlopt-2.4.1-x86_64
# TCL = $(HOME)/sfw/tcl-8.6.1
TCL = /usr
# TCL = $(HOME)/sfw/tcl-8.5.9-x86_64
TCLSH = $(wildcard $(TCL)/bin/tclsh8.* )
TCL_INCLUDE = /usr/include/tcl
TCL_LIB =  /usr/lib/x86_64-linux-gnu

# Set Tcl version here: 
TCL_VERSION = 8.5
# TCL_VERSION = 8.6

INCLUDES = -I $(NLOPT)/include -I $(TCL_INCLUDE) -I src

CFLAGS += $(INCLUDES) -g -Wall -fPIC -std=gnu99

LDFLAGS =

LIBDIRS += -L $(NLOPT)/lib

LIBS += $(LIBDIRS) -lnlopt -lm

RPATH_NLOPT = -Wl,-rpath -Wl,$(NLOPT)/lib
RPATHS = $(RPATH_NLOPT) \
         -Wl,-rpath -Wl,$(PWD)/lib

all: bin/fop-nlopt

swift-pkg: pkgIndex.tcl

sanity:
#       Test that we have NLopt
	test -d $(NLOPT)

bin:
	mkdir -pv bin

lib:
	mkdir -pv lib

LIBNFHEDM = lib/libnfhedm.so
LIBTCLNFHEDM = lib/libtclnfhedm.so

NFHEDM_NLOPT_OBJS = src/FitOrientationParametersNLOpt.o \
                 src/FitOrientation.o                \
                 src/SharedFuncsFit.o                \
                 src/CalcDiffractionSpots.o          \
                 src/swift-bindings.o                \
                 src/placebo.o                       \
                 src/parameters.o

HEADERS = src/checks.h src/SharedFuncsFit.h src/CalcDiffractionSpots.h \
           src/swift-bindings.h

src/CalcDiffractionSpots.o: src/CalcDiffractionSpots.c $(HEADERS)

src/FitOrientationParametersNLOpt-main.o:        \
	src/FitOrientationParametersNLOpt-main.c \
	$(HEADERS)
	gcc -c $(CFLAGS) -o $(@) $(<)

bin/fop-nlopt:  $(LIBNFHEDM)                                           \
                src/FitOrientationParametersNLOpt-main.o
	@mkdir -pv bin
	gcc $(LDFLAGS)                                           \
		-o $(@) src/FitOrientationParametersNLOpt-main.o \
		-L lib -l nfhedm                                    \
		$(LIBS)                                          \
		$(RPATHS)                                        \

bin/fo-nlopt:  $(LIBNFHEDM)                                           \
                src/FitOrientation-main.o
	@mkdir -pv bin
	gcc $(LDFLAGS)                                           \
		-o $(@) src/FitOrientation-main.o \
		-L lib -l nfhedm                                    \
		$(LIBS)                                          \
		$(RPATHS) 

$(LIBNFHEDM): sanity $(NFHEDM_NLOPT_OBJS) lib
	gcc -shared -o $(@)       \
		$(NFHEDM_NLOPT_OBJS) \
		$(LIBS)           \
		$(RPATH_NLOPT)

$(LIBTCLNFHEDM): sanity src/nfhedm_wrap.o $(NFHEDM_NLOPT_OBJS)
	gcc -shared -o $(@)         \
		src/nfhedm_wrap.o  \
		$(NFHEDM_NLOPT_OBJS)   \
		$(LIBS)             \
		-L $(TCL_LIB) -l tcl$(TCL_VERSION) \
		$(RPATH_NLOPT) -Wl,-rpath -Wl,$(TCL)/lib

src/nfhedm_wrap.c: src/nfhedm.i
	swig $(<)

pkgIndex.tcl: $(LIBTCLNFHEDM) make-package.tcl
	LEAF_PKG=tclnfhedm LEAF_VERSION=0.0  \
	LEAF_SO=$(<) LEAF_TCL=tcl/nfhedm.tcl \
	$(TCLSH) make-package.tcl > $(@)

TESTS = tests/test-placebo.x

tests: $(TESTS)

%.x: %.o $(LIBNFHEDM)
	gcc -o $(@) $(<)      \
		-L lib -l nfhedm \
		$(RPATHS)

clean:
	@echo CLEAN
	rm -fv bin/fop-nlopt bin/fo-nlopt
	rm -fv src/*.o
	rm -fv src/nfhedm_wrap.c
	rm -fv lib/libtclnfhedm.so lib/libnfhedm.so
	rm -fv $(TESTS)
	@echo

.PHONY: sanity clean all
