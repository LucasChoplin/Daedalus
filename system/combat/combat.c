#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../structs.h"
#include "combat.h"
#include "combat_attaque.h"
#include "combat_aff.h"
#include "../inventaire.h"
#include "../text.h"

/** \file combat.c
    \brief contenus des fonctions de combat.h
    \author Lucas Choplin
    \version 1.0
    \date 8 février 2026
*/

const SDL_Rect fuite = { 800, 900, 425, 40 };
const SDL_Rect attack_btn = { 800, 850, 200, 40 };
const SDL_Rect forte = {1025, 850, 200, 40 };
const SDL_Rect inventaire = { 800, 800, 425, 40 };

int is_point_in_rect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

int lancerCombat(SDL_Renderer *renderer, Fighter *joueur, Mob *ennemi, item_t * listeItem[]) {

    int x=10,y=10;
    int xp=0;
    TTF_Font* font = getEndScreenFont();

    GameState state = PLAYER;
    int running = 1;
    int inv = 0; //variable pour savoir si on affiche l'inventaire ou pas
    
    while (running &&(joueur->hp>0 && ennemi->hp>0)) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
            
            // 1. DÉTECTION DU CLAVIER (Indépendant du clic)
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_TAB) {
                    inv = !inv; // Alterne l'affichage de l'inventaire
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && state == PLAYER) {
                int mx = e.button.x;
                int my = e.button.y;                

                
                if (is_point_in_rect(mx, my, attack_btn)) {
                    attaque(joueur, ennemi, &state);
                }

                if (is_point_in_rect(mx, my, forte)) {
                    attaqueForte(joueur, ennemi, &state);
                }


                if (is_point_in_rect(mx, my, fuite)) {
                    if(joueur->speed>ennemi->speed){
                        return 0;
                    }
                    else{
                        state = ENNEMY;
                    }
                }
                if((inv)&&(detecterItemUtilise(&e,listeItem,joueur))){
                    inv = !inv;
                    state = ENNEMY;
                }

                if (is_point_in_rect(mx, my, inventaire)) {
                    inv = !inv;//si on appuie sur inventaire on change la valeur de inv 
                }
            }

        }



        if (state == ENNEMY) {
            SDL_Delay(500);
            attaqueEnnemi(joueur, ennemi, &state);
        }

        if(state==VICTOIRE){
            srand( time( NULL ) );
            xp= (rand() % (20 - 10 + 1)) + 10;
            joueur->xp+=xp;

        }

        // RENDER
        afficherCombat(renderer, joueur, *ennemi, fuite, attack_btn, forte, inventaire, inv, listeItem);
        SDL_Delay(16);
    }
    SDL_Delay(2000);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    int itemDrop[6] = {0,0}; //tableau pour stocker les items obtenus
    if(state == VICTOIRE){
        dropItem(listeItem,itemDrop,joueur,1);
    }
    endScreen(renderer,state, &x ,&y, font, xp,itemDrop);
    if(ennemi){
        ennemi->vaincu = (state == VICTOIRE) ? 1 : 0;
    }
    return 0;
}