
tests/test-read.o: io.o sro.o # Module deps

tests/test-read.x: tests/test-read.o io.o sro.o
	gfortran -o $(@) $(^) \
		$(HDF_LIBS)

tests/test-read-expermnt.x: tests/test-read-exprmnt.o io.o sro.o 
	gcc -o $(@) $(^) \
		$(HDF_LIBS) -lgfortran -lm
