#include <stdio.h>
#include <SDL2/SDL.h>
#include "inventaire.h"
#include "atlas.h"
/** \file Inventaire.c
    \brief contenus des fonctions de inventaire.h.
    \author Patrick Leguillon
    \version 1.0
    \date 20 février 2026
*/
static SDL_Texture* atlasItem = NULL;

void initAtlasItem(SDL_Renderer * r){
    //atlasItem = IMG_LoadTexture(r, "Img/Item.png");
    chargerImage("Img/potion.bmp",r,&atlasItem);
}

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
    SDL_Rect TexItem;
    for(int i =0; i<NB_ITEM;i++){
        if(l[i]->nb>0){
            TexItem = getTileRect(i);
            SDL_RenderCopy(r, atlasItem,&TexItem, &item);
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

int dropItem(item_t * l[],int ennemi, SDL_Texture ** t){
    switch (ennemi){
        case 0:
            if(rand()%10>=9){
                l[0]->nb++;
                //(*t) = l[0]->t;
                return 1;
            }
            break;
    }
    return 0;
}

int afficherItemObtenu(SDL_Renderer * r, SDL_Texture * t, int time){
    SDL_Rect d = {300,1000 - time*4,TAILLE_ITEM,TAILLE_ITEM};
    SDL_Rect TexItem = getTileRect(0);
    SDL_RenderCopy(r,atlasItem,&TexItem,&d);
    printf("%d",time);
    if(time>1000){
        return 0;
    }
    time++;
    return time;
}