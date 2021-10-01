EXECUTABLE=p1

shell: p0.c 
	gcc -o $(EXECUTABLE) p1.c headers.h list.c list.h 

