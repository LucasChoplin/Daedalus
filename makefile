bin/start : bin/main.o bin/map.o bin/atlas.o bin/inventaire.o bin/utilitaire.o bin/text.o bin/combat.o bin/combat_aff.o bin/combat_attaque.o
	gcc bin/main.o bin/map.o bin/atlas.o bin/inventaire.o bin/utilitaire.o bin/text.o bin/combat.o bin/combat_aff.o bin/combat_attaque.o -o bin/start -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

bin/main.o : src/main.c lib/structs.h lib/def.h
	gcc -c src/main.c -o bin/main.o
	
bin/map.o : src/map.c lib/map.h lib/atlas.h lib/structs.h lib/def.h
	gcc -c src/map.c -o bin/map.o

bin/atlas.o : src/atlas.c lib/atlas.h lib/structs.h lib/def.h
	gcc -c src/atlas.c -o bin/atlas.o

bin/inventaire.o : src/inventaire.c lib/inventaire.h lib/structs.h lib/def.h
	gcc -c src/inventaire.c -o bin/inventaire.o

bin/utilitaire.o: src/utilitaire.c lib/utilitaire.h lib/structs.h lib/def.h
	gcc -c src/utilitaire.c -o bin/utilitaire.o

bin/combat.o : src/combat.c lib/combat.h lib/structs.h lib/def.h
	gcc -c src/combat.c -o bin/combat.o

bin/combat_aff.o : src/combat_aff.c lib/combat_aff.h lib/structs.h lib/def.h
	gcc -c src/combat_aff.c -o bin/combat_aff.o

bin/combat_attaque.o : src/combat_attaque.c lib/combat_attaque.h lib/structs.h lib/def.h
	gcc -c src/combat_attaque.c -o bin/combat_attaque.o
	
bin/text.o : src/text.c lib/text.h lib/structs.h lib/def.h
	gcc -c src/text.c -o bin/text.o

clean:
	rm -f bin/*.o start