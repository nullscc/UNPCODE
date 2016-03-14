include ../config

PROGS = clichat srvchat 

all:${PROGS}

clichat:clichat.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

srvchat:srvchat.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f ${PROGS} *.o
