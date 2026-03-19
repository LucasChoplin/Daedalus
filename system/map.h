#ifndef map_h //evite les inclusions multiples
#define map_h
#include <SDL2/SDL.h>
#include "../def.h"
#include "../structs.h"

/** \file map.h
    \brief librairie pour générer la map et déplacer le personnage 
    \author Myriam Laaqira
    \version 1.0
    \date février ??
*/

int isWall(int tile);
void initMap(void);
int getIDSalleRandom(void);
void drawMap(SDL_Renderer* renderer);
void drawMob(SDL_Renderer* renderer, Mob* mob);
void cleanupMap(void);
void changeRoom(int direction, int* playerX, int* playerY);

extern Salle* currentMap;
extern Salle* salles[];

#endif