#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utilitaire.h"
#include "../def.h"
/** \file Inventaire.c
    \brief contenus des fonctions de utilitaire.h.
    \author Patrick Leguillon
    \version 1.0
    \date 28 février 2026
*/
static SDL_Texture * atlasMenu = NULL;//texture qui sert à stocker l'atlas des menus 

void initAtlasMenu(SDL_Renderer * r){
    atlasMenu = IMG_LoadTexture(r, "Img/bouton.png");
}

SDL_Texture * getAtlasMenu(void){
    return atlasMenu;
}

void cleanupAtlasMenu(void){
    SDL_DestroyTexture(atlasMenu);
}

SDL_Rect getTileRect2(int ID,int TAILLE_ATLAS){
    SDL_Rect rect;
    rect.x = (ID % TAILLE_ATLAS) * 64;
    rect.y = (ID / TAILLE_ATLAS) * 64;
    rect.w = 64;
    rect.h = 64;
    return rect;
}

int FichierExiste(char nom[]){
    FILE * f = fopen(nom, "r");
    if(f){
        fclose(f);
        return 1;
    }
    return 0;
}

void chargerImage(char nom[],SDL_Renderer * r,SDL_Texture ** t){
    SDL_Surface * s = SDL_LoadBMP(nom);
    SDL_SetColorKey(s, SDL_TRUE, SDL_MapRGB(s->format, 255, 255, 255));
    (*t) = SDL_CreateTextureFromSurface(r,s);
    SDL_FreeSurface(s);
}

int detecterButtonClique(SDL_Event * e,SDL_Rect * r){
    SDL_Point p = {e->button.x,e->button.y};//je crée un point à partir des coordonnées de la souris 
    return SDL_PointInRect(&p,r); 
    //j'utilise la fonction PointInRect qui vérifie si p et dans le rectangle de l'image pour Rect r
}

void afficherChiffre(SDL_Renderer * r,SDL_Texture * t,int nb,SDL_Rect * d){
    d->w = 32;
    d->h = 32;
    SDL_Rect chif = {nb%10*32,0,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    if(nb>9){
        SDL_Rect chif2 = {(nb/10)*32,0,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
        SDL_Rect d2 = {d->x-32,d->y,32,32};
        SDL_RenderCopy(r, t,&chif2,&d2);
    }
    SDL_RenderCopy(r, t,&chif,d);
}

void sauvegarder(Fighter p,item_t * l[]){
    FILE * f = fopen(FICHIER_DATA,"w");
    fprintf(f,"pv_max=%d\n",p.max_hp);
    fprintf(f,"stat_attaque=%d\n",p.attack);
    fprintf(f,"stat_speed=%d\n",p.speed);
    fprintf(f,"nb_potions=%d\n",l[0]->nb);
    fprintf(f,"nb_Superpotions=%d\n",l[1]->nb);
    fclose(f);
}