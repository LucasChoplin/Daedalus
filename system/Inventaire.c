#include <stdio.h>
#include <SDL2/SDL.h>
#include "inventaire.h"
/** \file Inventaire.c
    \brief contenus des fonctions de inventaire.h.
    \author Patrick Leguillon
    \version 1.0
    \date 20 février 2026
*/

void soin1PV(Fighter * p){
    p->hp++;
    if(p->hp>p->max_hp){
        p->hp = p->max_hp;
    }
}

void soin5PV(Fighter * p){
    p->hp+= 5;
    if(p->hp>p->max_hp){
        p->hp = p->max_hp;
    }
}

void affficherIventaire(SDL_Renderer * r, SDL_Texture * t, item_t * l[]){
    int x = 100;
    SDL_Rect item = {100,100,TAILLE_ITEM,TAILLE_ITEM};
    SDL_Rect c = {100,120,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    for(int i =0; i<NB_ITEM;i++){
        if(l[i]->nb>0){
            SDL_RenderCopy(r, l[i]->t, NULL, &item);
            afficherChiffre(r,t,l[i]->nb,&c);
            x+=100;
            item.x = x;
            c.x = x;
        }
    }
}

void detecterItemUtilise(SDL_Event * event, item_t * l[],Fighter*p){
    int x = 100;
    SDL_Rect item = {100,100,TAILLE_ITEM,TAILLE_ITEM};
    for(int i = 0;i < NB_ITEM;i++){
        if(l[i]->nb>0){
            if(detecterButtonClique(event,&item)){
                l[i]->nb--;
                l[i]->f(p);
            }
            x+=100;
            item.x = x;
        }
    }
}

void FreeTextureItem(item_t * l[]){
    for(int i = 0; i < NB_ITEM;i++){
        SDL_DestroyTexture(l[i]->t);
    }
}