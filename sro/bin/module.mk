
bin/sro: src/f/main.o $(OBJS_CORE)
	@ mkdir -pv bin
	h5fc -o $(@) $(^)

clean::
	rm -fv bin/sro
