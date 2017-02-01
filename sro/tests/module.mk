
tests/test-read.o: io.o sro.o # Module deps

tests/test-read.x: tests/test-read.o io.o sro.o
	gfortran -o $(@) $(^) \
		$(HDF_LIBS)
