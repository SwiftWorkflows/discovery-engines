
CC = h5cc
FLAGS = -Wall -g

ifeq ($(FAST),1)
	FLAGS += -O3
endif
ifeq (py,$(findstring py,$(MAKECMDGOALS)))
	FFLAGS += -fPIC
	CFLAGS += -fPIC
endif

FFLAGS = $(FLAGS) -fimplicit-none -Wuninitialized -pedantic -cpp
CFLAGS = $(FLAGS) -I .

all: sro

%.o: %.f90
	h5fc -c $(FFLAGS) $(<)

main.o: sro.o # Module dependency

sro: main.o sro.o
	h5fc -o $(@) $(^)

test-c-sro.x: test-c-sro.o
	$(CC) -o $(@) $(^)

clean:
	rm -fv sro *.o
