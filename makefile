EXECUTABLE=p2
CFLAGS= -Wall -g

shell: p2.c
	gcc $(CFLAGS) -o $(EXECUTABLE) p2.c commands.c headers.h list.c list.h auxFunc.c
