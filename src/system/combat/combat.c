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
#include <SDL2/SDL_mixer.h>

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
    SDL_Rect destEchap = {1180,50,TAILLE_SPRITE/2,TAILLE_SPRITE/2};//position du bouton echap
    int game =1;
    int x=10,y=10;
    int xp=0,gold=15;
    TTF_Font* font = getEndScreenFont();

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur SDL_mixer: %s\n", Mix_GetError());
    }

    Mix_Music *OST = Mix_LoadMUS("assets/God_s Falling.mp3");

    if (!OST) {
        printf("Erreur chargement musique: %s\n", Mix_GetError());
    }

    Mix_PlayMusic(OST, -1);

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
                if(inv && detecterButtonClique(&e,&destEchap)){
                    inv = 0; //ferme l'inventaire si on clique sur le bouton echap
                }
                
                if (is_point_in_rect(mx, my, attack_btn)) {
                    attaque(joueur, ennemi, &state);
                }

                if (is_point_in_rect(mx, my, forte)) {
                    attaqueForte(joueur, ennemi, &state);
                }


                if (is_point_in_rect(mx, my, fuite)) {
                    if(joueur->speed>ennemi->speed){
                        return -1;
                    }
                    else{
                        state = ENNEMY;
                    }
                }
                // if((inv)&&(detecterItemUtiliseCombat(&e,listeItem,joueur))){
                //     inv = !inv;
                //     state = ENNEMY;
                // }

                if (is_point_in_rect(mx, my, inventaire)) {
                    inv = !inv;//si on appuie sur inventaire on change la valeur de inv 
                }
            }

        }

        afficherCombat(renderer, joueur, *ennemi, fuite, attack_btn, forte, inventaire, inv, listeItem);
        SDL_Delay(16);


        if (state == ENNEMY && ennemi->hp > 0) {
            SDL_Delay(1000);
            attaqueEnnemi(joueur, ennemi, &state);
        }

        if(state==VICTOIRE){
            float puissance_ennemi = (ennemi->attack * 1.5) + (ennemi->max_hp * 0.5) + ennemi->speed;
            xp = (int)(puissance_ennemi / 5);
            xp += (rand() % 5);
            if((joueur->xp+=xp)>=joueur->max_xp){
                joueur->lvl++;
                joueur->xp=0;
                joueur->max_xp*=1.25;
                float pv_perc = (float)joueur->hp / joueur->max_hp;
                switch(joueur->classeID){
                    case GLADIATEUR:
                        joueur->max_hp *=1.25;
                        joueur->attack *=1.1;
                        joueur->speed *= 1.1;
                        break;
                    case ARCHER:
                        joueur->max_hp *=1.1;
                        joueur->attack *=1.25;
                        joueur->speed *= 1.1;
                        break;
                    case LANCIER:
                        joueur->max_hp *=1.1;
                        joueur->attack *=1.1;
                        joueur->speed *= 1.25;
                        break;
                }
                joueur->hp = (int)(joueur->max_hp * pv_perc);
            }
            joueur->gold+=gold;
            game=0;
        }

        // RENDER
        if(game){
            afficherCombat(renderer, joueur, *ennemi, fuite, attack_btn, forte, inventaire, inv, listeItem);
            SDL_Delay(16);
        }

    }

    Mix_FreeMusic(OST);
    Mix_CloseAudio();
    SDL_Delay(2000);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    loot_t drop; //tableau pour stocker les items obtenus
    if(state == VICTOIRE){
        drop = dropItem(listeItem,1);
        drop.xp = xp;
        drop.or = gold;
    }
    endScreen(renderer,state, &x ,&y, font,&drop);
    return joueur->hp;
}