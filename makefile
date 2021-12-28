EXECUTABLE=shell
CFLAGS= -Wall -g

shell: p4.c
	gcc $(CFLAGS) -o $(EXECUTABLE) p4.c commands.c headers.h list.c list.h auxFunc.c
