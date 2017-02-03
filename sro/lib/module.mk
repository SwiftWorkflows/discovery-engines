
$(LIBPY): src/swig/sro_wrap.o src/c/sro-c-iface.o $(OBJS_CORE)
	@ mkdir -pv lib
	gcc -shared -o $(@) $(^) \
		-L /usr/lib/x86_64-linux-gnu -l python2.7 \
		-l gfortran \
		$(HDF_LIBS)

clean::
	rm -fv lib/*.so
