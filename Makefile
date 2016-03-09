all:
	make -C ./lib
	make -C ./time_get
	make -C ./echo

clean:
	make -C ./lib clean
	make -C ./time_get clean
	make -C ./echo clean
	rm -f libzwunp.a