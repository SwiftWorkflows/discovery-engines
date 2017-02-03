
tests-c: tests/c/test-c-sro.x

tests/c/%.x: tests/c/%.o src/c/sro-c-iface.o $(OBJS_CORE)
	$(CLINKER) -o $(@) $(^) \
		$(HDF_LIBS) -lgfortran -lm

clean::
	rm -fv tests/c/*.x tests/c/*.o

.PHONY: tests-c
