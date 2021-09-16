EXECUTABLE=shell

shell: shell.c 
	gcc -o $(EXECUTABLE) shell.c list.c list.h 
