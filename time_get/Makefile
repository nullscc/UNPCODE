all: clitime servtime

clitime:clitime.c
	gcc -o clitime clitime.c
	
servtime:servtime.c
	gcc -o servtime servtime.c

.PHONY: all clean

clean:
	rm cli.out serv.out