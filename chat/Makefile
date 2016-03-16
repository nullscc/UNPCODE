include ../config

PROGS = clichat srvchat 

all:${PROGS}

clichat:clichat.o chatfunc.o cmd.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

srvchat:srvchat.o chatfunc.o cmd.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

chatfunc.o:cmd.o
clean:
	rm -f ${PROGS} *.o
