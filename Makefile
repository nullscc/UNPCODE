all:
	make -C ./lib
	make -C ./time_get

clean:
	make -C ./lib clean
	make -C ./time_get clean
	rm -f libzwunp.a