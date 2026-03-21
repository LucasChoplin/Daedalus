#ifndef map_h //evite les inclusions multiples
#define map_h
#include <SDL2/SDL.h>
#include "../def.h"
#include "../structs.h"

/** \file map.h
    \brief librairie pour générer la map et déplacer le personnage 
    \author Myriam Laaqira
    \version 1.0
    \date 14 février 2026
*/

/** \brief verifie si une tile est un mur
    \param tile numero de la tile a verifier
    \return 1 si c'est un mur 0 sinon
*/
int isWall(int tile);

/** \brief verifie si une tile est un sol de combat
    \param tile numero de la tile a verifier
    \return 1 si c'est un sol de combat 0 sinon
*/
int isCombatTile(int tile);

/** \brief initialise un etage entier 
*/
void initMap(void);

/** \brief retourne l'ID d'une salle aléatoire
    \return ID de la salle
*/
int getIDSalleRandom(void);

/** \brief dessine la map actuelle (l'etage)
    \param renderer pointeur vers le renderer
*/
void drawMap(SDL_Renderer* renderer);

/** \brief dessine un mob sur la map
    \param renderer pointeur vers le renderer
    \param mob pointeur vers le mob a dessiner
*/
void drawMob(SDL_Renderer* renderer, Mob* mob);

/** \brief nettoie la map actuelle
*/
void cleanupMap(void);

/** \brief change l'affichage de la salle en fct de la direction du perso
    \param direction direction du changement de salle
    \param playerX pointeur vers la position X du joueur
    \param playerY pointeur vers la position Y du joueur
*/
void changeRoom(int direction, int* playerX, int* playerY);

extern Salle* currentMap;
extern Salle* salles[];

#endif