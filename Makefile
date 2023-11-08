all: geass

geass: main.o
	gcc main.o -o geass

main.o: main.c
	gcc -c main.c