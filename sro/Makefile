
FFLAGS = -fimplicit-none -ffree-form

all: sro

%.o: %.f90
	h5fc -c -g $(FFLAGS) $(<)

main.o: sro.o # Module dependency

sro: main.o sro.o
	h5fc -o $(@) $(^)
