include ../config

PROGS = clichat srvchat 

all:${PROGS}

clichat:clichat.o chatfunc.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

srvchat:srvchat.o chatfunc.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f ${PROGS} *.o
