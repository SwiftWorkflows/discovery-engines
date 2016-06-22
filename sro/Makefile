
FFLAGS = -fimplicit-none

all: sro

%.o: %.f90
	gfortran -c -g $(FFLAGS) $(<)

sro: main.o sro.o
	gfortran -o $(@) $(^)
