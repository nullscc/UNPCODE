PROGS  = clichat srvchat 
OBJECT = chatfunc.o cmd.o wrapsock.o wrapio.o
CC = gcc

all:${PROGS}

clichat:clichat.o ${OBJECT}
	${CC} ${CFLAGS} -o $@ $^

srvchat:srvchat.o ${OBJECT}
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f ${PROGS} *.o
