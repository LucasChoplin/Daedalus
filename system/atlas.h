#ifndef atlas_h //evite les inclusions multiples
#define atlas_h
#include <SDL2/SDL.h>
#include "../def.h"

void initAtlas(SDL_Renderer* renderer);
void initMobAtlas(SDL_Renderer* renderer);
/** \brief initialise l'atlas des items  
    \param t pointeur vers le moteur de rendu 
*/
void initAtlasItem(SDL_Renderer * r);
void initAtlasMenu(SDL_Renderer * r);
void initAtlasPerso(SDL_Renderer * r);

void cleanupAtlas(void);
void cleanupMobAtlas(void);
void cleanupAtlasItem(void);
void cleanupAtlasMenu(void);
void cleanupAtlasPerso(void);

SDL_Texture* getAtlasTexture(void);
SDL_Texture* getMobAtlasTexture(void);
SDL_Texture* getAtlasItem(void);
SDL_Texture * getAtlasMenu(void);
SDL_Texture * getAtlasPerso(void);

SDL_Rect getTileRect(int ID);
SDL_Rect getTileRect2(int ID,int TAILLE_ATLAS);

#endif 
