
NLOPT = $(HOME)/sfw/nlopt-2.4.1

INCLUDES = -I $(NLOPT)/include -I src

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

FOP_NLOPT_OBJS = src/FitOrientationParametersNLOpt.o \
	         src/CalcDiffractionSpots.o          \
	         src/nfhedm-math.o                   \
		 src/parameters.o                    \
                 src/placebo.o

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

lib/libfop.so: $(FOP_NLOPT_OBJS) lib
	gcc -shared -o $(@) $(FOP_NLOPT_OBJS) $(LIBS) $(RPATH_NLOPT)

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
	rm -fv lib/libfop.so
	rm -fv $(TESTS)
	@echo

.PHONY: clean all

