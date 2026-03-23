#ifndef combat_aff_H
#define combat_aff_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../structs.h"
#include <stdlib.h>
#include <stdio.h>
/** \file combat_aff.h
    \brief librairie qui gère les visuels de combat 
    \author Lucas Choplin
    \version 1.0
    \date février ??
*/

// afficher 

/** \param inv si =1 on affiche l'inventaire
    \param l pointeur vers la liste d'items pour afficher l'inventaire 
*/
void afficherCombat(SDL_Renderer *renderer, Fighter *joueur, Mob ennemi,SDL_Rect fuite,SDL_Rect attack_btn,SDL_Rect forte,SDL_Rect inventaire,int inv,item_t * l[]);

void endScreen(SDL_Renderer *renderer, GameState state, int* x, int* y ,TTF_Font* font);
#endif