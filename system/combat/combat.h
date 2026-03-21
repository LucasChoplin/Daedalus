#ifndef COMBAT_H
#define COMBAT_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../structs.h"
#include "../../def.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

/** \file combat.h
    \brief librairie qui gère les combats
    \author Lucas Choplin
    \version 1.0
    \date février ??
*/

int lancerCombat(SDL_Renderer *renderer, Fighter *joueur, item_t * listeItem[], SDL_Texture * item);

#endif