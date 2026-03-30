#ifndef structs_h //evite les inclusions multiples
#define structs_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "def.h"

/** \file structs.h
    \brief contient les structs du projets
    \author Myriam Laaqira
    \version 1.0
    \date 12 février 2026
*/

typedef struct{
    SDL_Renderer *renderer;
    SDL_Window *window;
} Game;

typedef struct{
    int xTile, yTile, facing;
    int spriteID;
    SDL_Texture *texture;
} Player;

typedef struct{
    int mapID, xSalle, ySalle;
    int tiles[SALLE_HEIGHT][SALLE_WIDTH];
    int itemAtlasID;
    int itemX;
    int itemY;
    SalleType type;
} Salle;

typedef struct{
    int mapID, spriteID, xTile, yTile;
    int hp, max_hp, attack, speed;
} Mob;

typedef struct {
    int classeID;/** indique la classe du personnage */
    int hp;
    int max_hp;
    int attack;
    int speed;
    int max_xp;
    int xp;
    int gold;
    int lvl;
} Fighter;


/** \brief structure pour les items  */
typedef struct{
    int nb;/**< quantité de l'item */
    void (*f) (Fighter * p);/* pointeur vers la fonction pour utiliser un item*/ 
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