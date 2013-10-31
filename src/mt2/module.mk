
DIR := src/mt2

BINS += bin/mt2

# LIBS := -L $(HDF)/lib

# RPATHS := -Wl,-rpath $(HDF)/lib

bin/mt2: $(DIR)/mt2.o
	gcc -o $(@) $(<) $(LIBS) -l hdf5 -l tiff \
	    $(RPATHS)

clean::
	rm -fv $(DIR)/*.o
