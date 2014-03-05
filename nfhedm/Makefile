
NLOPT = $(HOME)/sfw/nlopt-2.4.1

INCLUDES = -I $(NLOPT)/include

CFLAGS += $(INCLUDES) -g -fPIC -std=gnu99

LDFLAGS =

LIBDIRS += -L $(NLOPT)/lib

LIBS += $(LIBDIRS) -lnlopt -lm

RPATHS = -Wl,-rpath -Wl,$(NLOPT)/lib

all: bin/fop-nlopt

bin:
	mkdir -pv bin

lib:
	mkdir -pv lib

FOP_NLOPT_OBJS = src/FitOrientationParametersNLOpt.o src/CalcDiffractionSpots.o

src/FitOrientationParametersNLOpt-main.o:        \
	src/FitOrientationParametersNLOpt-main.c \
	src/FitOrientationParametersNLOpt.c
	gcc -c $(CFLAGS) -o $(@) $(<)

bin/fop-nlopt:  src/CalcDiffractionSpots.o               \
                src/FitOrientationParametersNLOpt-main.o \
		bin
	gcc $(LDFLAGS) -o $(@) src/FitOrientationParametersNLOpt-main.o src/CalcDiffractionSpots.o  $(LIBS) $(RPATHS)

lib/libfop.so: $(FOP_NLOPT_OBJS) lib
	gcc -shared -o $(@) $(FOP_NLOPT_OBJS) $(LIBS) $(RPATHS)

clean:
	rm -fv bin/fop-nlopt
	rm -fv src/*.o
