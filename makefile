EXECUTABLE=p0

shell: p0.c 
	gcc -o $(EXECUTABLE) p0.c headers.h list.c list.h 

