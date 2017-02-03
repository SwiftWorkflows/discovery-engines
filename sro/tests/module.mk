
tests/test-read.o: io.o sro.o # Module deps

tests/test-read.x: tests/test-read.o $(OBJS_CORE)
	gfortran -o $(@) $(^) \
		$(HDF_LIBS)

tests/%.x: tests/%.o sro-helpers.o $(OBJS_CORE)
	gcc -o $(@) $(^) \
		$(HDF_LIBS) -lgfortran -lm
