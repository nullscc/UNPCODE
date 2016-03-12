include ../config

PROGS = cliecho srvecho srvecho_select srvecho_poll

all:${PROGS}

cliecho:cliecho.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

srvecho:srvecho.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

srvecho_select:srvecho_select.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^

srvecho_poll:srvecho_poll.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^
	
clean:
	rm -f ${PROGS} *.o