#ifndef COMBAT_attack_H
#define COMBAT_attack_H
#include "../../structs.h"
#include "../../def.h"
#include <SDL2/SDL.h>

// attaque normale 
void attaque(Fighter *joueur, Fighter *ennemi, GameState *state);

// attaque forte
void attaqueForte(Fighter *joueur, Fighter *ennemi, GameState *state);

// attaque de l'ennemi
void attaqueEnnemi(Fighter *joueur, Fighter *ennemi, GameState *state);

#endif