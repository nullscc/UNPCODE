PROGS  = clichat srvchat testchat
OBJECT = chatfunc.o cmd.o wrapsock.o wrapio.o
CC = gcc -I../inc

all:${PROGS}

clichat:clichat.o ${OBJECT}
	${CC} ${CFLAGS} -o $@ $^

srvchat:srvchat.o ${OBJECT}
	${CC} ${CFLAGS} -o $@ $^

testchat:testchat.o ${OBJECT}
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f ${PROGS} *.o
