#ifndef atlas_h //evite les inclusions multiples
#define atlas_h
#include <SDL2/SDL.h>
#define TILE_SIZE 64
#define ATLAS_COLUMNS 2

void initAtlas(SDL_Renderer* renderer);
void cleanupAtlas(void);

SDL_Rect getTileRect(int ID);
SDL_Texture* getAtlasTexture(void);

#endif 
