bin/start : lib/main.o lib/map.o lib/atlas.o lib/inventaire.o lib/utilitaire.o lib/text.o lib/combat.o lib/combat_aff.o lib/combat_attaque.o
	gcc lib/main.o lib/map.o lib/atlas.o lib/inventaire.o lib/utilitaire.o lib/text.o lib/combat.o lib/combat_aff.o lib/combat_attaque.o -o bin/start -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

lib/main.o : src/main.c src/structs.h src/def.h
	gcc -c src/main.c -o lib/main.o
	
lib/map.o : src/system/map.c src/system/map.h src/system/atlas.h src/structs.h src/def.h
	gcc -c src/system/map.c -o lib/map.o

lib/atlas.o : src/system/atlas.c src/system/atlas.h src/structs.h src/def.h
	gcc -c src/system/atlas.c -o lib/atlas.o

lib/inventaire.o : src/system/inventaire.c src/system/inventaire.h src/structs.h src/def.h
	gcc -c src/system/inventaire.c -o lib/inventaire.o

lib/utilitaire.o: src/system/utilitaire.c src/system/utilitaire.h src/structs.h src/def.h
	gcc -c src/system/utilitaire.c -o lib/utilitaire.o

lib/combat.o : src/system/combat/combat.c src/system/combat/combat.h src/structs.h src/def.h
	gcc -c src/system/combat/combat.c -o lib/combat.o

lib/combat_aff.o : src/system/combat/combat_aff.c src/system/combat/combat_aff.h src/structs.h src/def.h
	gcc -c src/system/combat/combat_aff.c -o lib/combat_aff.o

lib/combat_attaque.o : src/system/combat/combat_attaque.c src/system/combat/combat_attaque.h src/structs.h src/def.h
	gcc -c src/system/combat/combat_attaque.c -o lib/combat_attaque.o
	
lib/text.o : src/system/text.c src/system/text.h src/structs.h src/def.h
	gcc -c src/system/text.c -o lib/text.o

clean:
	rm -f lib/*.o start