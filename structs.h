#ifndef structs_h //evite les inclusions multiples
#define structs_h
#include "def.h"

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
    int mapID, tileX, tileY;
} Mob;

typedef struct {
    int hp;
    int max_hp;
    int attack;
    int speed;
} Fighter;

/** \brief structure pour les items  */
typedef struct{
    int nb;/**< nom de l'image de l'item*/
    SDL_Texture * t;/**< quantité de l'item */
    void (*f) (Fighter * p);/* pointeur vers la fonction pour utiliser un item*/ 
} item_t;

extern Game game;

#endif