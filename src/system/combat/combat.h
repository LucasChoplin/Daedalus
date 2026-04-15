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
    \date 8 février 2026
*/

/** \brief fonction qui lance le combat 
    \param renderer pointeur vers le moteur de rendu
    \param joueur pointeur vers le struct figther du personnage
    \param ennemi pointeur vers la structure mob des ennemis
    \param listeItem liste des item contenu dans l'inventaire
*/
int lancerCombat(SDL_Renderer *renderer, Fighter *joueur, Mob *ennemi, item_t * listeItem[]);

#endif