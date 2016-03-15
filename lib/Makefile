include ../config

LIB_OBJS = wrapsock.o wrapio.o wrapsignal.o

all:${LIB_OBJS}
	ar rv ../libzwunp.a $^
	${RANLIB} ../libzwunp.a
	
clean:
	rm -f ../libzwunp.a *.o
