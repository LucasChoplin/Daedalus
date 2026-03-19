#ifndef structs_h //evite les inclusions multiples
#define structs_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "def.h"

/** \file combat_aff.c
    \brief contenus des fonctions de combat_aff.h 
    \author Lucas Choplin
    \version 1.0
    \date février ??
*/

typedef struct{
    SDL_Renderer *renderer;
    SDL_Window *window;
} Game;

typedef struct{
    int xTile, yTile, facing;
    SDL_Texture *texture;
} Player;

typedef struct{
    int mapID, xSalle, ySalle;
    int tiles[SALLE_HEIGHT][SALLE_WIDTH];
} Salle;

typedef struct{
    int mapID, xTile, yTile;
} Mob;

typedef struct {
    int classeID;/**< indique la classe du personnage */
    int hp;
    int max_hp;
    int attack;
    int speed;
    int xp;
} Fighter;

/** \brief structure pour les items  */
typedef struct{
    int nb;/**< quantité de l'item */
    void (*f) (Fighter * p);/**< pointeur vers la fonction pour utiliser un item*/ 
} item_t;

typedef struct{
    SDL_Rect rect;
    SDL_Color couleurFond;
    SDL_Color couleurTexte;
    char *texte;
    TTF_Font *font;
}Bouton;

extern Game game;

#endif