#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "inventaire.h"
#include "atlas.h"
#include "../def.h"
#include "text.h"
/** \file inventaire.c
    \brief contenus des fonctions de inventaire.h
    \author Patrick Leguillon
    \version 1.0
    \date 20 février 2026
*/

void soin50PV(Fighter * p){
    p->hp+= 50;
    if(p->hp>p->max_hp){
        p->hp = p->max_hp;
    }
}

void soin200PV(Fighter * p){
    p->hp+= 200;
    if(p->hp>p->max_hp){
        p->hp = p->max_hp;
    }
}

void augmenterPvmax(Fighter * p){
    p->max_hp += 10;
}

void augmenterAttaque(Fighter * p){
    p->attack += 10;
}

void augmenterVitesse(Fighter * p){
    p->speed += 10;
}

void afficherPiece(SDL_Renderer * r,int nbPiece, int x, int y){
    SDL_Rect src = getItemRect(ID_PIECE);
    SDL_Rect dest = {x+100,y,TAILLE_AFF_ITEM,TAILLE_AFF_ITEM};
    drawChiffre(r,nbPiece,x,y);
    SDL_RenderCopy(r,getAtlasItem(),&src,&dest);
}

void afficherXp(SDL_Renderer * r,int xp, int x, int y){
    SDL_Color titleColor = {240, 230, 180, 255};
    drawChiffre(r,xp,x,y);
    drawText(game.renderer, getTitleFont(), "XP", titleColor, x +100, y);
}

void afficherInventaire(SDL_Renderer * r, item_t * l[],Fighter * p){
    SDL_Rect destMenu = {50,50,SCREEN_WIDTH/2,SCREEN_HEIGHT-50};
    SDL_Rect menuF = getTileRect(5,ATLAS_BOUTON);
    SDL_Rect Perso = {SCREEN_WIDTH/1.5,SCREEN_HEIGHT/3,TAILLE_SPRITE,TAILLE_SPRITE};
    SDL_Rect pv = {SCREEN_WIDTH/1.7,SCREEN_HEIGHT/3+128,TAILLE_SPRITE,TAILLE_SPRITE};
    SDL_Rect persoF = getTileRect(p->classeID,ATLAS_PERSO);
    int x = 100;
    int y = 100;
    SDL_Rect item = {100,100,TAILLE_AFF_ITEM,TAILLE_AFF_ITEM};
    SDL_Rect c = {150,120,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    SDL_Rect TexItem;
    SDL_RenderCopy(r,getAtlasMenu(),&menuF,&destMenu);
    SDL_RenderCopy(r,getAtlasPerso(),&persoF,&Perso);
    drawChiffre(r,p->hp,pv.x,pv.y);
    pv.x +=200;
    drawChiffre(r,p->max_hp,pv.x,pv.y);
    pv.y +=100;
    pv.x -=200;
    drawChiffre(r,p->xp,pv.x,pv.y);
    pv.x +=200;
    afficherXp(r,p->max_xp,pv.x,pv.y);
    pv.y +=100;
    pv.x -=200;
    drawChiffre(r,p->attack,pv.x,pv.y);

    for(int i =0; l[i]!=NULL;i++){
        if(l[i]->nb>0){
            TexItem = getItemRect(i);
            SDL_RenderCopy(r, getAtlasItem(),&TexItem, &item);
            drawChiffre(r,l[i]->nb,c.x,c.y);
            x+=100;
            item.x = x;
            c.x = x +50;
            if(i%5==0 && i != 0){
                y += 100;
                item.y = y;
                c.y = y ;
                x = 100;
                item.x = x;
                c.x = x +50;
            }
        }
    }
    afficherPiece(r,p->gold,SCREEN_WIDTH*0.8,SCREEN_HEIGHT*0.1);
}

void combat_afficher_inventaire(SDL_Renderer * r, item_t * l[]){
    SDL_Rect destMenu = {50,50,SCREEN_WIDTH/1.1,SCREEN_HEIGHT-50};
    SDL_Rect menuF = getTileRect(5,ATLAS_BOUTON);
    SDL_Rect item = {100,100,TAILLE_AFF_ITEM,TAILLE_AFF_ITEM};
    SDL_Rect c = {110,120,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    SDL_Rect TexItem;
    SDL_RenderCopy(r,getAtlasMenu(),&menuF,&destMenu);
    int x = 100;
    for(int i =0; l[i]!=NULL;i++){
        if(l[i]->nb>0){
            TexItem = getItemRect(i);
            SDL_RenderCopy(r, getAtlasItem(),&TexItem, &item);
            drawChiffre(r,l[i]->nb,c.x,c.y);
            x+=100;
            item.x = x;
            c.x = x+10;
        }
    }
}

int detecterItemUtilise(SDL_Event * event, item_t * l[], Fighter*p){
    int x = 100;
    SDL_Rect item = {100,100,TAILLE_AFF_ITEM,TAILLE_AFF_ITEM};
    for(int i = 0;l[i]!=NULL;i++){
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

void dropItem2(item_t * l[], int ennemi, loot_t * d){
    switch (ennemi){
        case 1:
            if(rand()%10>=5){
                l[0]->nb++;
                d->item[d->nbItem] = 0;
                d->nbItem++;
            }
            if(rand()%20>=5){
                l[1]->nb++;
                d->item[d->nbItem] = 1;
                d->nbItem++;
            }
            if(rand()%30>=29){
                l[3]->nb++;
                d->item[d->nbItem] = 3;
                d->nbItem++;
            }
            if(rand()%50>=49){
                switch(rand()%3){
                    case 0: l[4]->nb++;
                        d->item[d->nbItem] = 4;
                        break;
                    case 1: l[5]->nb++;
                        d->item[d->nbItem] = 5;
                        break;
                    case 2: l[6]->nb++;
                        d->item[d->nbItem] = 6;
                }
                d->nbItem++;
            }
            break;
    }
}

loot_t dropItem(item_t * l[], int e1/*,int e2,int e3,int e4, int e5, int e6*/){
    loot_t drop;
    drop.nbItem = 0;
    drop.xp = 0;
    drop.or = 0;
    if(e1!=0){
        dropItem2(l,e1,&drop);
    }
    return drop;
    /*if(e2!=0){
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
    }*/
}

loot_t dropCoffre(item_t * l[],Fighter * p){
    loot_t drop;
    drop.nbItem = 0;
    drop.or = 0;
    switch(rand()%4){
        case 0: l[0]->nb+=10;
            drop.nbItem = 10;
            for (int i =0;i<10;i++){
                drop.item[i]=0;
            }
            break;
        case 1: l[1]->nb+=5;
            drop.nbItem = 5;
            for (int i =0;i<5;i++){
                drop.item[i]=1;
            }
            break;
        case 2: p->gold+=200;
            drop.or = 200;
            break;
        case 3: 
            for(int i = 0;i<3;i++){
                switch(rand()%3){
                    case 0: l[4]->nb++;
                        drop.item[drop.nbItem] = 4;
                        break;
                    case 1: l[5]->nb++;
                        drop.item[drop.nbItem] = 5;
                        break;
                    case 2: l[6]->nb++;
                        drop.item[drop.nbItem] = 6;
                }
                drop.nbItem ++;
            }
    }
    switch(rand()%3){
        case 0: l[4]->nb++;
            drop.item[drop.nbItem] = 4;
            break;
        case 1: l[5]->nb++;
            drop.item[drop.nbItem] = 5;
            break;
        case 2: l[6]->nb++;
            drop.item[drop.nbItem] = 6;
    }
    drop.nbItem++;
    return drop;
}

void afficherItemObtenuCombat(SDL_Renderer * r, loot_t * d){
    SDL_Rect destMenu = {50,50,1280-100,960-200};
    SDL_Rect destI ={100,SCREEN_HEIGHT-400,TAILLE_AFF_ITEM,TAILLE_AFF_ITEM};
    SDL_Rect destEchap = {1180,50,TAILLE_SPRITE/2,TAILLE_SPRITE/2};
    SDL_Rect imgEchap = getTileRect(4,ATLAS_BOUTON);
    SDL_Rect imgMenu = getTileRect(5,ATLAS_BOUTON);
    SDL_Rect imgItem;
    //SDL_RenderCopy(r,getAtlasMenu(),&imgMenu,&destMenu);
    //SDL_RenderCopy(r,getAtlasMenu(),&imgEchap,&destEchap);
    for(int i = 0;i<d->nbItem;i++){
        imgItem = getItemRect(d->item[i]);
        SDL_RenderCopy(r,getAtlasItem(),&imgItem,&destI);
        destI.x += 100;
    }
    afficherXp(r,d->xp,SCREEN_WIDTH*0.1,SCREEN_HEIGHT*0.7);
    afficherPiece(r,d->or,SCREEN_WIDTH*0.1,SCREEN_HEIGHT*0.8);
}

void afficherItemObtenu(SDL_Renderer * r, loot_t * d){
    SDL_Rect destMenu = {50,50,1280-100,960-200};
    SDL_Rect destI ={100,SCREEN_HEIGHT*0.1,TAILLE_AFF_ITEM,TAILLE_AFF_ITEM};
    SDL_Rect destEchap = {1180,50,TAILLE_SPRITE/2,TAILLE_SPRITE/2};
    SDL_Rect imgEchap = getTileRect(4,ATLAS_BOUTON);
    SDL_Rect imgMenu = getTileRect(5,ATLAS_BOUTON);
    SDL_Rect imgItem;
    SDL_RenderCopy(r,getAtlasMenu(),&imgMenu,&destMenu);
    SDL_RenderCopy(r,getAtlasMenu(),&imgEchap,&destEchap);
    for(int i = 0;i<d->nbItem;i++){
        imgItem = getItemRect(d->item[i]);
        SDL_RenderCopy(r,getAtlasItem(),&imgItem,&destI);
        destI.x += 100;
    }
    if(d->or>0){
        afficherPiece(r,d->or,SCREEN_WIDTH*0.1,SCREEN_HEIGHT*0.3);
    }
}

void afficherMagasin(SDL_Renderer * r,Fighter * p){
    SDL_Rect destMenu = {50,50,SCREEN_WIDTH/1.1,SCREEN_HEIGHT-50};
    SDL_Rect destEchap = {1180,50,TAILLE_SPRITE/2,TAILLE_SPRITE/2};
    SDL_Rect imgEchap = getTileRect(4,ATLAS_BOUTON);
    SDL_Rect menuF = getTileRect(5,ATLAS_BOUTON);
    SDL_Rect item = {100,100,TAILLE_AFF_ITEM,TAILLE_AFF_ITEM};
    SDL_Rect c = {110,120,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    SDL_Rect TexItem;
    int prixItem[2] = {1,15};
    SDL_RenderCopy(r,getAtlasMenu(),&menuF,&destMenu);
    SDL_RenderCopy(r,getAtlasMenu(),&imgEchap,&destEchap);
    int x = 100;
    for(int i =0; i<2;i++){
            TexItem = getItemRect(i);
            SDL_RenderCopy(r, getAtlasItem(),&TexItem, &item);
            drawChiffre(r,prixItem[i],c.x,c.y);
            x+=100;
            item.x = x;
            c.x = x+10;
    }
    drawChiffre(r,p->gold,SCREEN_WIDTH/1.1,SCREEN_HEIGHT/10);
}

void detecterAchat(SDL_Event * event, Fighter * p, item_t * l[]){
    SDL_Rect item = {100,100,TAILLE_AFF_ITEM,TAILLE_AFF_ITEM};
    int prixItem[2] = {1,15};
    int x = 100;
    for(int i = 0;i<2;i++){
        if(detecterButtonClique(event,&item)){
            if(p->gold>=prixItem[i]){
                p->gold-=prixItem[i];
                l[i]->nb++;
            }
        }
        x+=100;
        item.x = x;
    }
}