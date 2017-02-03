
src/swig/sro_wrap.c: src/swig/sro.i src/c/sro.h
	swig -python $(<)

clean::
	rm -fv src/swig/*.o src/swig/*_wrap.c
