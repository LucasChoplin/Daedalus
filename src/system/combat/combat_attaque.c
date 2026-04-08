#include "combat_aff.h"
#include <SDL2/SDL.h>
#include "../../structs.h"
#include "../../def.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/** \file combat_attaque.h
    \brief contenus des fonctions de combat_attaque.h
    \author Lucas Choplin
    \version 1.0
    \date 8 février 2026
*/

// attaque normale 
void attaque(Fighter *joueur, Mob *ennemi, GameState *state){
    if(ennemi->hp<(30*joueur->attack)/100){
        ennemi->hp=0;
    }
    else
        ennemi->hp -= (30*joueur->attack)/100;
    *state = (ennemi->hp <= 0) ? VICTOIRE : ENNEMY;
}


// attaque forte
void attaqueForte(Fighter *joueur, Mob *ennemi, GameState *state){
    if(joueur->pm_atk>0){
        if(ennemi->hp<(50*joueur->attack)/100){
            ennemi->hp=0;
        }
        else{
            ennemi->hp -= (50*joueur->attack)/100;
        }
        joueur->pm_atk-=1;
        *state = (ennemi->hp <= 0) ? VICTOIRE : ENNEMY;
    }
}

// attaque de l'ennemi
void attaqueEnnemi(Fighter *joueur, Mob *ennemi, GameState *state){
    srand( time( NULL ) );
    int degats;
    if(ennemi->hp>0){
        switch(rand()%2) {
        case 0:
            degats = (100 * ennemi->attack) / 100;
            joueur->hp = (joueur->hp < degats) ? 0 : joueur->hp - degats; break;
        case 1:
            degats = (100 * ennemi->attack) / 100;
            joueur->hp = (joueur->hp < degats) ? 0 : joueur->hp - degats; break;
    }
    }           
    *state = (joueur->hp <= 0) ? DEFAITE : PLAYER;
}