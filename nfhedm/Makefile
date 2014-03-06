
NLOPT = $(HOME)/sfw/nlopt-2.4.1

INCLUDES = -I $(NLOPT)/include

CFLAGS += $(INCLUDES) -g -Wall -fPIC -std=gnu99

LDFLAGS =

LIBDIRS += -L $(NLOPT)/lib

LIBS += $(LIBDIRS) -lnlopt -lm

RPATHS = -Wl,-rpath -Wl,$(NLOPT)/lib

all: bin/fop-nlopt

bin:
	mkdir -pv bin

lib:
	mkdir -pv lib

libfop: lib/libfop.so

FOP_NLOPT_OBJS = src/FitOrientationParametersNLOpt.o \
	         src/CalcDiffractionSpots.o          \
	         src/nfhedm-math.o                   \
		 src/parameters.o

HEADERS = src/checks.h src/nfhedm-math.h

src/CalcDiffractionSpots.o: src/CalcDiffractionSpots.c $(HEADERS)

src/nfhedm-math.o: src/nfhedm-math.c src/nfhedm-math.h

src/FitOrientationParametersNLOpt-main.o:        \
	src/FitOrientationParametersNLOpt-main.c \
	$(HEADERS)
	gcc -c $(CFLAGS) -o $(@) $(<)

bin/fop-nlopt:  bin libfop                                       \
                src/FitOrientationParametersNLOpt-main.o
	gcc $(LDFLAGS)                                           \
		-o $(@) src/FitOrientationParametersNLOpt-main.o \
		-L lib -l fop                                    \
		$(LIBS)                                          \
		$(RPATHS)                                        \
		-Wl,-rpath -Wl,$(PWD)/lib

lib/libfop.so: $(FOP_NLOPT_OBJS) lib
	gcc -shared -o $(@) $(FOP_NLOPT_OBJS) $(LIBS) $(RPATHS)

clean:
	@echo CLEAN
	rm -fv bin/fop-nlopt
	rm -fv src/*.o
	@echo

