#ifndef map_h //evite les inclusions multiples
#define map_h
#include <SDL2/SDL.h>
#define MAP_WIDTH 20
#define MAP_HEIGHT 15

int isWall(int tile);
void drawMap(SDL_Renderer* renderer, int offsetX, int offsetY);

extern int map[MAP_HEIGHT][MAP_WIDTH];

#endif