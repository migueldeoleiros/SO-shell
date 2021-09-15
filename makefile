EXECUTABLE=shell

shell: shell.c 
	gcc -o $(EXECUTABLE) shell.c -lreadline 
