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
/** \brief initialise l'atlas des menus
    \param t pointeur vers le moteur de rendu 
*/
void initAtlasMenu(SDL_Renderer * r);
/** \brief initialise l'atlas des Persos
    \param t pointeur vers le moteur de rendu 
*/
void initAtlasPerso(SDL_Renderer * r);

void cleanupAtlas(void);
void cleanupMobAtlas(void);
/** \brief supprime l'atlas des items 
*/
void cleanupAtlasItem(void);
/** \brief supprime l'atlas des menus 
*/
void cleanupAtlasMenu(void);
/** \brief supprime l'atlas des persos 
*/
void cleanupAtlasPerso(void);

SDL_Texture* getAtlasTexture(void);
SDL_Texture* getMobAtlasTexture(void);
/** \brief renvoie l'addresse de l'atlas des items
*/
SDL_Texture* getAtlasItem(void);
/** \brief renvoie l'addresse de l'atlas des menus 
*/
SDL_Texture * getAtlasMenu(void);
/** \brief renvoie l'addresse de l'atlas des persos
*/
SDL_Texture * getAtlasPerso(void);

SDL_Rect getTileRect(int ID);

/** \brief renvoie le découpage de la case numéro ID dans un atals de taille TAILLE_ATLAS * TAILLE_ATLAS 
    \param ID numéro de l'image choisi partant de 0 à gauche en haut
    \param TAILLE_ATLAS taille d'un des cotés de l'atlas sachant qu'un atlas est un carré 
*/
SDL_Rect getTileRect2(int ID,int TAILLE_ATLAS);

#endif 
