EXECUTABLE=shell
CFLAGS= -Wall -g

shell: p3.c
	gcc $(CFLAGS) -o $(EXECUTABLE) p3.c commands.c headers.h list.c list.h auxFunc.c
