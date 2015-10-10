
THIS := src/mt2

BINS += bin/mt2

bin/mt2: $(THIS)/mt2.o
	mkdir -pv bin
	gcc -o $(@) $(<) \
		-L $(HDF)/lib -l hdf5 -l tiff \
		-Wl,-rpath -Wl,$(HDF)/lib

$(THIS)/mt2.o: $(THIS)/util.h

clean::
	rm -fv $(THIS)/*.o
