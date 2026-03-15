#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../structs.h"
#include "combat.h"
#include "combat_aff.h"
#include "../inventaire.h"


const SDL_Rect fuite = { 800, 900, 425, 40 };
const SDL_Rect attack_btn = { 800, 850, 200, 40 };
const SDL_Rect forte = {1025, 850, 200, 40 };
const SDL_Rect inventaire = { 800, 800, 425, 40 };

int is_point_in_rect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}


int lancerCombat(SDL_Renderer *renderer){

    int x=10,y=10;

    TTF_Init();

    TTF_Font* font = TTF_OpenFont("times.ttf", 100);

    Fighter joueur = {100, 100, 120, 100, 60};
    Fighter ennemi  = {80,  80,  115, 20};

    GameState state = PLAYER;
    int running = 1;
    
    while (running &&(joueur.hp>0 && ennemi.hp>0)) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;

            if (e.type == SDL_MOUSEBUTTONDOWN && state == PLAYER) {
                int mx = e.button.x;
                int my = e.button.y;                

                if (is_point_in_rect(mx, my, attack_btn)) {
                    if(ennemi.hp<50*(joueur.attack/100)){
                        ennemi.hp=0;
                    }
                    else
                        ennemi.hp -= 50*(joueur.attack/100);
                    state = (ennemi.hp <= 0) ? VICTOIRE : ENNEMY;
                }

                if (is_point_in_rect(mx, my, forte)) {
                    if(ennemi.hp<30*(joueur.attack/100)){
                        ennemi.hp=0;
                    }
                    else{
                        ennemi.hp -= 30*(joueur.attack/100);
                    }
                    state = (ennemi.hp <= 0) ? VICTOIRE : ENNEMY;
                }


                if (is_point_in_rect(mx, my, fuite)) {
                    if(joueur.speed>ennemi.speed){
                        TTF_CloseFont(font);
                        TTF_Quit();
                        return 0;
                    }
                    else{
                        state = ENNEMY;
                    }
                }
            }

        }



        if (state == ENNEMY) {
            SDL_Delay(500);
            switch(rand()%2) {
                case 0:joueur.hp = (joueur.hp<30*(ennemi.attack/100)) ? 0 : joueur.hp-30*(ennemi.attack/100);break;
                case 1:joueur.hp = (joueur.hp<50*(ennemi.attack/100)) ? 0 : joueur.hp-50*(ennemi.attack/100);break;
            }
                   
            state = (joueur.hp <= 0) ? DEFAITE : PLAYER;
        }

        // RENDER
        afficherCombat(renderer, &joueur, &ennemi, fuite, attack_btn, forte, inventaire);
        SDL_Delay(16);
    }
    SDL_Delay(2000);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    EndScreen(renderer,state, &x ,&y, font);

    TTF_CloseFont(font);
    TTF_Quit();
    return 0;
}

