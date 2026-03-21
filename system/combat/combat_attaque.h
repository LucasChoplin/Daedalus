#ifndef COMBAT_ATTAQUE_H
#define COMBAT_ATTAQUE_H    
#include "../../structs.h"
#include "../../def.h"
#include <SDL2/SDL.h>

/** \file combat_attaque.h
    \brief librairie qui gère les attaques 
    \author Lucas Choplin
    \version 1.0
    \date février ??
*/

// attaque normale 
void attaque(Fighter *joueur, Mob *ennemi, GameState *state);

// attaque forte
void attaqueForte(Fighter *joueur, Mob *ennemi, GameState *state);

// attaque de l'ennemi
void attaqueEnnemi(Fighter *joueur, Mob *ennemi, GameState *state);

#endif