start : main.o map.o atlas.o inventaire.o utilitaire.o text.o combat.o combat_aff.o combat_attaque.o
	gcc main.o map.o atlas.o inventaire.o utilitaire.o text.o combat.o combat_aff.o combat_attaque.o -o start -lSDL2 -lSDL2_image -lSDL2_ttf

main.o : main.c structs.h def.h
	gcc -c main.c
	
map.o : system/map.c system/map.h system/atlas.h structs.h def.h
	gcc -c system/map.c

atlas.o : system/atlas.c system/atlas.h def.h
	gcc -c system/atlas.c

inventaire.o : system/inventaire.c system/inventaire.h structs.h def.h
	gcc -c system/inventaire.c

utilitaire.o: system/utilitaire.c system/utilitaire.h structs.h def.h
	gcc -c system/utilitaire.c

combat.o : system/combat/combat.c system/combat/combat.h structs.h def.h
	gcc -c system/combat/combat.c

combat_aff.o : system/combat/combat_aff.c system/combat/combat_aff.h structs.h def.h
	gcc -c system/combat/combat_aff.c

combat_attaque.o : system/combat/combat_attaque.c system/combat/combat_attaque.h structs.h def.h
	gcc -c system/combat/combat_attaque.c

text.o : system/text.c system/text.h structs.h def.h
	gcc -c system/text.c

clean:
	rm -f *.o start