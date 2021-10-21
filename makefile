EXECUTABLE=p1
CFLAGS= -Wall

shell: p1.c 
	gcc $(CFLAGS) -o $(EXECUTABLE) p1.c commands.c headers.h list.c list.h 

