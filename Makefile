all: geass target

target: target.c
	gcc target.c -o target -fno-stack-protector -z execstack -no-pie

geass: main.o
	gcc main.o -o geass

main.o: main.c
	gcc -c main.c

clean:
	rm *.o geass target