all: cli serv

cli:clitime.c
	gcc -o cli.out clitime.c
	
serv:servtime.c
	gcc -o serv.out servtime.c

.PHONY: all clean serv cli

clean:
	rm cli.out serv.out