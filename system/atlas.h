#ifndef atlas_h //evite les inclusions multiples
#define atlas_h
#include <SDL2/SDL.h>
#include "../def.h"

void initAtlas(SDL_Renderer* renderer);
void initMobAtlas(SDL_Renderer* renderer);

void cleanupAtlas(void);
void cleanupMobAtlas(void);

SDL_Rect getTileRect(int ID);

SDL_Texture* getAtlasTexture(void);
SDL_Texture* getMobAtlasTexture(void);

#endif 
