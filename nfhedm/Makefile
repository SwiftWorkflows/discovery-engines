
# Set software locations here:
# NLOPT = $(HOME)/sfw/nlopt-2.4.1
NLOPT = $(HOME)/sfw/nlopt-2.4.1-x86_64
# TCL = $(HOME)/sfw/tcl-8.6.1
TCL = $(HOME)/sfw/tcl-8.5.9-x86_64
TCLSH = $(wildcard $(TCL)/bin/tclsh8.* )

INCLUDES = -I $(NLOPT)/include -I $(TCL)/include -I src

CFLAGS += $(INCLUDES) -g -Wall -fPIC -std=gnu99

LDFLAGS =

LIBDIRS += -L $(NLOPT)/lib

LIBS += $(LIBDIRS) -lnlopt -lm

RPATH_NLOPT = -Wl,-rpath -Wl,$(NLOPT)/lib
RPATHS = $(RPATH_NLOPT) \
         -Wl,-rpath -Wl,$(PWD)/lib

all: sanity bin/fop-nlopt

sanity:
#       Test that we have NLopt
	test -d $(NLOPT)

bin:
	mkdir -pv bin

lib:
	mkdir -pv lib

LIBFOP = lib/libfop.so
LIBTCLFOP = lib/libtclfop.so

FOP_NLOPT_OBJS = src/FitOrientationParametersNLOpt.o src/nfhedm-math.o src/CalcDiffractionSpots.o src/placebo.o src/parameters.o
#
#  \
# 	                   \
# 	                            \
# 		                     \

HEADERS = src/checks.h src/nfhedm-math.h

src/CalcDiffractionSpots.o: src/CalcDiffractionSpots.c $(HEADERS)

src/nfhedm-math.o: src/nfhedm-math.c src/nfhedm-math.h

src/FitOrientationParametersNLOpt-main.o:        \
	src/FitOrientationParametersNLOpt-main.c \
	$(HEADERS)
	gcc -c $(CFLAGS) -o $(@) $(<)

bin/fop-nlopt:  $(LIBFOP)                                           \
                src/FitOrientationParametersNLOpt-main.o
	@mkdir -pv bin
	gcc $(LDFLAGS)                                           \
		-o $(@) src/FitOrientationParametersNLOpt-main.o \
		-L lib -l fop                                    \
		$(LIBS)                                          \
		$(RPATHS)                                        \

$(LIBFOP): $(FOP_NLOPT_OBJS) lib
	gcc -shared -o $(@)       \
		$(FOP_NLOPT_OBJS) \
		$(LIBS)           \
		$(RPATH_NLOPT)

$(LIBTCLFOP): src/placebo_wrap.o $(FOP_NLOPT_OBJS)
	gcc -shared -o $(@)         \
		$(<)                \
		$(FOP_NLOPT_OBJS)   \
		$(LIBS)             \
		-L $(TCL)/lib -l tcl8.5 \
		$(RPATH_NLOPT) -Wl,-rpath -Wl,$(TCL)/lib

src/placebo_wrap.c: src/placebo.i
	swig $(<)

pkgIndex.tcl: $(LIBTCLFOP) make-package.tcl
	LEAF_PKG=tclfop LEAF_VERSION=0.0  \
	LEAF_SO=$(<) LEAF_TCL=tcl/fop.tcl \
	$(TCLSH) make-package.tcl > $(@)

TESTS = tests/test-placebo.x

tests: $(TESTS)

%.x: %.o $(LIBFOP)
	gcc -o $(@) $(<)      \
		-L lib -l fop \
		$(RPATHS)

clean:
	@echo CLEAN
	rm -fv bin/fop-nlopt
	rm -fv src/*.o
	rm -fv src/placebo_wrap.c
	rm -fv lib/libfop.so
	rm -fv $(TESTS)
	@echo

.PHONY: clean all

