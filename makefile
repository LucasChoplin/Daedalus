CC = gcc
CFLAGS = `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

all: main

monprog: main.c
	$(CC) main.c -o main $(CFLAGS) $(LDFLAGS)

clean:
	rm -f main
