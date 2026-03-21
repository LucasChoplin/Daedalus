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
void afficherCombat(SDL_Renderer *renderer, Fighter *joueur, Fighter *ennemi,SDL_Rect fuite,SDL_Rect attack_btn,SDL_Rect forte,SDL_Rect inventaire);

void endScreen(SDL_Renderer *renderer, GameState state, int* x, int* y ,TTF_Font* font);
#endif