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
    \date 8 février 2026
*/

// afficher 

/** \brief affiche l'interface du combat avec les boutons et la barre de vie  
    \param renderer pointeur vers le moteur de rendu
    \param joueur pointeur vers le struct figther du personnage
    \param ennemi la structure mob des ennemis
    \param fuite coordone du bouton fuite
    \param attack_btn coordone du bouton d'attaque
    \param forte coordone du bouton de l'attaque forte
    \param inventaire coordone du bouton pour acceder à l'inventaire 
    \param inv si =1 on affiche l'inventaire
    \param l pointeur vers la liste d'items pour afficher l'inventaire 
*/
void afficherCombat(SDL_Renderer *renderer, Fighter *joueur, Mob ennemi,SDL_Rect fuite,SDL_Rect attack_btn,SDL_Rect forte,SDL_Rect inventaire,int inv,item_t * l[]);

/** \brief afficher l'écran de fin de combat
    \param renderer pointeur vers le moteur de rendu
    \param state structure du deroulement de la partie 
    \param x coordonnée x du texte de fin 
    \param y coordonnée y du texte de fin 
    \param font police d'écriture pour le texte 
    \param d pointeur vers le struct loot_t qui contient les informations sur les items,xp et or obtenus à la fin du combat
*/
void endScreen(SDL_Renderer *renderer, GameState state, int* x, int* y ,TTF_Font* font,loot_t * d);
#endif