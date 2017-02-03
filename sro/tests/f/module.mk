
tests-f: tests/f/test-read.x

tests/f/test-read.o: src/f/io.o
# src/f/sro-calc.o

tests/f/test-read.x: tests/f/test-read.o $(OBJS_CORE)
	$(FLINKER) -o $(@) $(^) \
		$(HDF_LIBS)

.PHONY: tests-f

clean::
	rm -fv tests/f/*.x tests/f/*.o
