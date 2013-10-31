
DIR := src/mt2

BINS += bin/mt2

bin/mt2: $(DIR)/mt2.o
	gcc -o $(@) $(<) -l hdf5
