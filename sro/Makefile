
CC = h5cc
FLAGS = -Wall -g

ifeq ($(FAST),1)
	FLAGS += -O3
endif
ifeq (py,$(findstring py,$(MAKECMDGOALS)))
	FLAGS += -fPIC
	INCLUDES += -I /usr/include/python2.7
endif

FFLAGS = $(FLAGS) -fimplicit-none -Wuninitialized -pedantic -cpp
CFLAGS = $(FLAGS) $(INCLUDES)

all: sro

py: _pysro.so

%.o: %.f90
	h5fc -c $(FFLAGS) $(<)

main.o: sro.o # Module dependency

sro: main.o sro.o
	h5fc -o $(@) $(^)

test-c-sro.x: test-c-sro.o sro.o
	$(CC) -o $(@) $(^) -lgfortran

sro_wrap.c: sro.i sro.h
	swig -python $(<)

_pysro.so: sro_wrap.o sro.o
	gcc -shared -o $(@) $(^) \
		-L /usr/lib/x86_64-linux-gnu -l python2.7 \
		-l gfortran

clean:
	rm -fv sro *.o
