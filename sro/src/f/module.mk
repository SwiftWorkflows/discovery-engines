
OBJS_CORE = src/f/sro-calc.o src/f/io.o src/f/sro-defn.o

# Module dependencies
src/f/main.o:     src/f/sro-calc.o
src/f/sro-calc.o: src/f/io.o
src/f/io.o:       src/f/sro-defn.o

# Default Fortran build rule
%.o: %.f90
	$(FC) -c -o $(@) $(<) \
		$(FFLAGS)

clean::
	rm -fv src/f/*.o
