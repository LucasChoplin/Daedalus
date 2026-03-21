#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "inventaire.h"
#include "atlas.h"
#include "../def.h"
#include "text.h"
/** \file inventaire.c
    \brief contient les structs du projets
    \author Myriam Laaqira
    \version 1.0
    \date février ?
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

void afficherInventaire(SDL_Renderer * r, item_t * l[],Fighter * p){
    SDL_Rect destMenu = {50,50,SCREEN_WIDTH/2,SCREEN_HEIGHT-50};
    SDL_Rect menuF = getTileRect(5,ATLAS_BOUTON);
    SDL_Rect Perso = {SCREEN_WIDTH/1.5,SCREEN_HEIGHT/3,TAILLE_SPRITE,TAILLE_SPRITE};
    SDL_Rect pv = {SCREEN_WIDTH/1.7,SCREEN_HEIGHT/3+128,TAILLE_SPRITE,TAILLE_SPRITE};
    SDL_Rect persoF = getTileRect(p->classeID,ATLAS_PERSO);
    int x = 100;
    SDL_Rect item = {100,100,TAILLE_ITEM,TAILLE_ITEM};
    SDL_Rect c = {110,120,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    SDL_Rect TexItem;
    SDL_RenderCopy(r,getAtlasMenu(),&menuF,&destMenu);
    SDL_RenderCopy(r,getAtlasPerso(),&persoF,&Perso);
    drawChiffre(r,p->hp,pv.x,pv.y);
    pv.x +=200;
    drawChiffre(r,p->hp,pv.x,pv.y);
    for(int i =0; i<NB_ITEM;i++){
        if(l[i]->nb>0){
            TexItem = getTileRect(i,ATLAS_ITEM);
            SDL_RenderCopy(r, getAtlasItem(),&TexItem, &item);
            drawChiffre(r,l[i]->nb,c.x,c.y);
            x+=100;
            item.x = x;
            c.x = x+10;
        }
    }
}

void combat_afficher_inventaire(SDL_Renderer * r, item_t * l[]){
    SDL_Rect destMenu = {50,50,SCREEN_WIDTH/1.1,SCREEN_HEIGHT-50};
    SDL_Rect menuF = getTileRect(5,ATLAS_BOUTON);
    SDL_Rect item = {100,100,TAILLE_ITEM,TAILLE_ITEM};
    SDL_Rect c = {110,120,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    SDL_Rect TexItem;
    SDL_RenderCopy(r,getAtlasMenu(),&menuF,&destMenu);
    int x = 100;
    for(int i =0; i<NB_ITEM;i++){
        if(l[i]->nb>0){
            TexItem = getTileRect(i,ATLAS_ITEM);
            SDL_RenderCopy(r, getAtlasItem(),&TexItem, &item);
            drawChiffre(r,l[i]->nb,c.x,c.y);
            x+=100;
            item.x = x;
            c.x = x+10;
        }
    }
}

int detecterItemUtilise(SDL_Event * event, item_t * l[],Fighter*p){
    int x = 100;
    SDL_Rect item = {100,100,TAILLE_ITEM,TAILLE_ITEM};
    for(int i = 0;i < NB_ITEM;i++){
        if(l[i]->nb>0){
            if(detecterButtonClique(event,&item)){
                if(l[i]->f != NULL){
                    l[i]->nb--;
                    l[i]->f(p);
                    return 1;
                }
            }
            x+=100;
            item.x = x;
        }
    }
    return 0;
}

int dropItem2(SDL_Renderer * r,int itemObtenu,item_t * l[],int ennemi, int itemdrop[]){
    SDL_Rect item = {100*(itemObtenu+1),100,TAILLE_ITEM,TAILLE_ITEM};
    SDL_Rect destItem;
    switch (ennemi){
        case 1:
            if(rand()%10>=9){
                l[0]->nb++;
                itemdrop[itemObtenu] = 0;
                itemObtenu++;
                itemdrop[itemObtenu] = 1;
                itemObtenu++;
            }
            break;
    }
    return itemObtenu;
}

int dropItem(SDL_Renderer * r,item_t * l[],int itemDrop[], int e1,int e2,int e3,int e4, int e5, int e6){
    int itemObtenu = 0;
    if(e1!=0){
        itemObtenu = dropItem2(r,itemObtenu,l,e1,itemDrop);
    }
    if(e2!=0){
        itemObtenu = dropItem2(r,itemObtenu,l,e2,itemDrop);
    }
    if(e3!=0){
        itemObtenu = dropItem2(r,itemObtenu,l,e3,itemDrop);
    }
    if(e4!=0){
        itemObtenu = dropItem2(r,itemObtenu,l,e4,itemDrop);
    }
    if(e5!=0){
        itemObtenu = dropItem2(r,itemObtenu,l,e5,itemDrop);
    }
    if(e6!=0){
        itemObtenu = dropItem2(r,itemObtenu,l,e6,itemDrop);
    }
    return itemObtenu;
}

void afficherItemObtenu(SDL_Renderer * r,int itemObtenu, int tItemObtenu[]){
    SDL_Rect destMenu = {50,50,1280-100,960-200};
    SDL_Rect destI ={100,100,TAILLE_ITEM,TAILLE_ITEM};
    SDL_Rect destEchap = {1180,50,TAILLE_SPRITE/2,TAILLE_SPRITE/2};
    SDL_Rect imgEchap = getTileRect(4,ATLAS_BOUTON);
    SDL_Rect imgMenu = getTileRect(5,ATLAS_BOUTON);
    SDL_Rect imgItem;
    SDL_RenderCopy(r,getAtlasMenu(),&imgMenu,&destMenu);
    SDL_RenderCopy(r,getAtlasMenu(),&imgEchap,&destEchap);
    for(int i = 0;i<itemObtenu;i++){
        imgItem = getTileRect(tItemObtenu[i],ATLAS_ITEM);
        SDL_RenderCopy(r,getAtlasItem(),&imgItem,&destI);
        destI.x += 100;
    }
}