#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../structs.h"
#include "combat.h"
#include "combat_aff.h"
#include "combat_attaque.h"
#include "../inventaire.h"
#include "../text.h"
/** \file combat.c
    \brief contenus des fonctions de combat.h
    \author Lucas Choplin
    \version 1.0
    \date février ??
*/


const SDL_Rect fuite = { 800, 900, 425, 40 };
const SDL_Rect attack_btn = { 800, 850, 200, 40 };
const SDL_Rect forte = {1025, 850, 200, 40 };
const SDL_Rect inventaire = { 800, 800, 425, 40 };

int is_point_in_rect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}


int lancerCombat(SDL_Renderer *renderer, Fighter *joueur, item_t * listeItem[], SDL_Texture * item){

    int x=10,y=10;
    TTF_Font* font = getEndScreenFont();

    Fighter ennemi  = {80,  80,  115, 20};

    GameState state = PLAYER;
    int running = 1;
    int inv = 0;//pour savoir si on doit afficher l'inventaire 
    while (running &&(joueur->hp>0 && ennemi.hp>0)) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;

            if (e.type == SDL_MOUSEBUTTONDOWN && state == PLAYER) {
                int mx = e.button.x;
                int my = e.button.y;                

                if (is_point_in_rect(mx, my, attack_btn)) {
                    attaque(joueur, &ennemi, &state);
                }

                if (is_point_in_rect(mx, my, forte)) {
                    attaqueForte(joueur, &ennemi, &state);
                }


                if (is_point_in_rect(mx, my, fuite)) {
                    if(joueur->speed>ennemi.speed){
                        return 0;
                    }
                    else{
                        state = ENNEMY;
                    }
                }
                if((inv)&&(detecterItemUtilise(&e,listeItem,joueur))){
                    //repasser au tour de l'ennemi car on vient d'utiliser un item
                }

                if (is_point_in_rect(mx, my, inventaire)) {
                    inv = !inv;//si on appuie sur inventaire on change la valeur de inv 
                }
            }

        }
        if (state == ENNEMY) {
            SDL_Delay(500);
            attaqueEnnemi(joueur, &ennemi, &state);
        }
        // RENDER
        afficherCombat(renderer, joueur, &ennemi, fuite, attack_btn, forte, inventaire,inv,listeItem);
        SDL_Delay(16);
    }
    SDL_Delay(2000);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    endScreen(renderer,state, &x ,&y, font);
    return 0;
}