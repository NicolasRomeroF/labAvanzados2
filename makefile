CC = gcc

puzzle: funciones.c puzzle.c
		$(CC) -o puzzle funciones.c puzzle.c -I.