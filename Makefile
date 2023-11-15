all: geass target

target: target.c
	gcc -o target target.c

geass: main.o
	gcc main.o -o geass

main.o: main.c
	gcc -c main.c

clean:
	rm *.o geass target