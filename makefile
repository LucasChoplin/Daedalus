start : main4.o map.o atlas.o inventaire.o utilitaire.o text.o combat.o combat_aff.o combat_attaque.o
	gcc main4.o map.o atlas.o inventaire.o utilitaire.o text.o combat.o combat_aff.o combat_attaque.o -o start -lSDL2 -lSDL2_image -lSDL2_ttf

main4.o : main4.c structs.h
	gcc -c main4.c
	
map.o : system/map.c system/map.h system/atlas.h
	gcc -c system/map.c

atlas.o : system/atlas.c system/atlas.h
	gcc -c system/atlas.c

inventaire.o : system/inventaire.c system/inventaire.h
	gcc -c system/inventaire.c

utilitaire.o: system/utilitaire.c system/utilitaire.h
	gcc -c system/utilitaire.c

combat.o : system/combat/combat.c system/combat/combat.h
	gcc -c system/combat/combat.c

combat_aff.o : system/combat/combat_aff.c system/combat/combat_aff.h
	gcc -c system/combat/combat_aff.c

combat_attaque.o : system/combat/combat_attaque.c system/combat/combat_attaque.h
	gcc -c system/combat/combat_attaque.c

text.o : system/text.c system/text.h
	gcc -c system/text.c