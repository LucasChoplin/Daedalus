#ifndef atlas_h //evite les inclusions multiples
#define atlas_h
#include <SDL2/SDL.h>
#include "../def.h"

/** \file atlas.h
    \brief librairie pour gérer les atlas 
    \author Myriam Laaqira
    \version 1.0
    \date 12 février 2026
*/

/** \brief initialise l'atlas de la map
    \param r pointeur vers le moteur de rendu 
*/
void initAtlasMap(SDL_Renderer* r);
/** \brief initialise l'atlas des mobs 
    \param r pointeur vers le moteur de rendu 
*/
void initAtlasMob(SDL_Renderer* r);
/** \brief initialise l'atlas des items  
    \param r pointeur vers le moteur de rendu 
*/
void initAtlasItem(SDL_Renderer * r);
/** \brief initialise l'atlas des menus
    \param r pointeur vers le moteur de rendu 
*/
void initAtlasMenu(SDL_Renderer * r);
/** \brief initialise l'atlas des Persos
    \param r pointeur vers le moteur de rendu 
*/
void initAtlasPerso(SDL_Renderer * r);

/**
 * \brief supprime l'atlas de la map
 */
void cleanupAtlasMap(void);
/** \brief supprime l'atlas des mobs 
*/
void cleanupAtlasMob(void);
/** \brief supprime l'atlas des items 
*/
void cleanupAtlasItem(void);
/** \brief supprime l'atlas des menus 
*/
void cleanupAtlasMenu(void);
/** \brief supprime l'atlas des persos 
*/
void cleanupAtlasPerso(void);

/** \brief renvoie l'addresse de l'atlas de la map
*/
SDL_Texture* getAtlasMap(void);
/** \brief renvoie l'addresse de l'atlas des mobs
*/
SDL_Texture* getAtlasMob(void);
/** \brief renvoie l'addresse de l'atlas des items
*/
SDL_Texture* getAtlasItem(void);
/** \brief renvoie l'addresse de l'atlas des menus 
*/
SDL_Texture * getAtlasMenu(void);
/** \brief renvoie l'addresse de l'atlas des persos
*/
SDL_Texture * getAtlasPerso(void);

/** \brief renvoie le découpage de la case numéro ID dans un atals de taille TAILLE_ATLAS * TAILLE_ATLAS (tiles 64x64)
    \param ID numéro de l'image choisi partant de 0 à gauche en haut
    \param tailleAtlas nombre de colonnes de l'atlas
*/
SDL_Rect getTileRect(int ID, int tailleAtlas);

/** \brief renvoie le Rect de le ID ème item d'un atlas en longueur de 32 pixels de hauteur
    \param ID numéro de l'image choisi en partant de 0 à gauche
*/
SDL_Rect getItemRect(int ID);

#endif 