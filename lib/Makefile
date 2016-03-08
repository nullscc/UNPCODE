include ../config

LIB_OBJS = wrapsock.o

all:${LIB_OBJS}
	ar rv ../libzwunp.a $?
	${RANLIB} ../libzwunp.a
	
clean:
	rm -f ../libzwunp.a *.o