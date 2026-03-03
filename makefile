start : main.o map.o atlas.o
	gcc main.o map.o atlas.o -o start -lSDL2 -lSDL2_image

main.o : main.c structs.h
	gcc -c main.c

map.o : system/map.c system/map.h system/atlas.h
	gcc -c system/map.c

atlas.o : system/atlas.c system/atlas.h
	gcc -c system/atlas.c

fight : combat.c
	gcc combat.c -o combat -lSDL2 -lSDL2_ttf

combat : fight
	./combat