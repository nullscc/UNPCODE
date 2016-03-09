include ../config

PROGS = cliecho srvecho

all:${PROGS}

cliecho:cliecho.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

srvecho:srvecho.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^
	
clean:
	rm -f ${PROGS} *.o