#ifndef COMBAT_ATTAQUE_H
#define COMBAT_ATTAQUE_H    
#include "../../structs.h"
#include "../../def.h"
#include <SDL2/SDL.h>

/** \file combat_attaque.h
    \brief librairie qui gère les attaques 
    \author Lucas Choplin
    \version 1.0
    \date 8 février 2026
*/

/** \brief inflige une attaque normal à l'ennemi
    \param joueur pointeur vers le struct figther du personnage
    \param ennemi pointeur vers la structure mob des ennemis
    \param state pointeur vers l'état actuel du combat
*/
void attaque(Fighter *joueur, Mob *ennemi, GameState *state);

/** \brief inflige une attaque forte à l'ennemi
    \param joueur pointeur vers le struct figther du personnage
    \param ennemi pointeur vers la structure mob des ennemis
    \param state pointeur vers l'état actuel du combat
*/
void attaqueForte(Fighter *joueur, Mob *ennemi, GameState *state);

/** \brief l'ennemi inflige des soit normale soit forte aléatoirement 
    \param joueur pointeur vers le struct figther du personnage
    \param ennemi pointeur vers la structure mob des ennemis
    \param state pointeur vers l'état actuel du combat
*/
void attaqueEnnemi(Fighter *joueur, Mob *ennemi, GameState *state);

#endif