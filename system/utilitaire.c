#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utilitaire.h"
#include "../def.h"
/** \file utilitaire.c
    \brief contenus des fonctions de utilitaire.h.
    \author Patrick Leguillon
    \version 1.0
    \date 28 février 2026
*/

int fichierExiste(char nom[]){
    FILE * f = fopen(nom, "r");
    if(f){
        fclose(f);
        return 1;
    }
    return 0;
}

int chargerImage(char nom[],SDL_Renderer * r,SDL_Texture ** t){
    SDL_Surface * s = SDL_LoadBMP(nom);
    SDL_SetColorKey(s, SDL_TRUE, SDL_MapRGB(s->format, 255, 255, 255));
    (*t) = SDL_CreateTextureFromSurface(r,s);
    SDL_FreeSurface(s);
    if(!(*t)){
        printf("Erreur de chargement de l'image %s",nom);
        return -1;
    }
    return 0;
}

int detecterButtonClique(SDL_Event * e,SDL_Rect * r){
    SDL_Point p = {e->button.x,e->button.y};//je crée un point à partir des coordonnées de la souris 
    return SDL_PointInRect(&p,r); 
    //j'utilise la fonction PointInRect qui vérifie si p est dans le rectangle de l'image pour Rect r
}

void afficherChiffre(SDL_Renderer * r,SDL_Texture * t,int nb,SDL_Rect * d){
    d->w = 32;
    d->h = 32;
    SDL_Rect chif = {nb%10*32,0,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    if(nb>9){
        SDL_Rect chif2 = {(nb%100/10)*32,0,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
        SDL_Rect d2 = {d->x-32,d->y,32,32};
        SDL_RenderCopy(r, t,&chif2,&d2);
        if(nb>99){
            chif2.x = nb%1000/100*32;
            d2.x = d->x-64;
            SDL_RenderCopy(r, t,&chif2,&d2);
        }
    }
    SDL_RenderCopy(r, t,&chif,d);
}

void sauvegarder(Fighter p,item_t * l[]){
    FILE * f = fopen(FICHIER_DATA,"w");
    fprintf(f,"classeID=%d\n",p.classeID);
    fprintf(f,"pv_max=%d\n",p.max_hp);
    fprintf(f,"stat_attaque=%d\n",p.attack);
    fprintf(f,"stat_speed=%d\n",p.speed);
    fprintf(f,"xp=%d\n",p.xp);
    fprintf(f,"niveau=%d\n",p.lvl);
    fprintf(f,"gold=%d\n",p.gold);
    fprintf(f,"nb_potions=%d\n",l[0]->nb);
    fprintf(f,"nb_Superpotions=%d\n",l[1]->nb);
    fprintf(f,"nb_PotionEnergie=%d\n",l[2]->nb);
    fprintf(f,"nb_clés=%d\n",l[3]->nb);
    fclose(f);
}