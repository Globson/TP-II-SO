all: main.c
	gcc main.c -o EXEC

clear: EXEC
	rm EXEC
