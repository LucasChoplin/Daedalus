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
    \param joueur structure des statistique du joueur
    \param ennemi structure des statistique de l'ennemi
    \param state valeur actulle sand le quel le combat est 
*/
void attaque(Fighter *joueur, Mob *ennemi, GameState *state);

/** \brief inflige une attaque forte à l'ennemi
    \param joueur structure des statistique du joueur
    \param ennemi structure des statistique de l'ennemi
    \param state valeur actulle sand le quel le combat est 
*/
void attaqueForte(Fighter *joueur, Mob *ennemi, GameState *state);

/** \brief l'ennemi inflige des soit normale soit forte aléatoirement 
    \param joueur structure des statistique du joueur
    \param ennemi structure des statistique de l'ennemi
    \param state valeur actulle sand le quel le combat est 
*/
void attaqueEnnemi(Fighter *joueur, Mob *ennemi, GameState *state);

#endif