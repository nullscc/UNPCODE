include ../config

PROGS = clitime servtime

all:${PROGS}
	
clitime:clitime.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^
servtime:servtime.o ../libzwunp.a
	${CC} ${CFLAGS} -o $@ $^


clean:
	rm -f ${PROGS} *.o