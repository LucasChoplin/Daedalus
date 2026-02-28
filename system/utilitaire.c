#include <stdio.h>
#include <SDL2/SDL.h>
#include "utilitaire.h"
/** \file Inventaire.c
    \brief contenus des fonctions de utilitaire.h.
    \author Patrick Leguillon
    \version 1.0
    \date 28 février 2026
*/

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
    SDL_Rect chif = {nb*32,0,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
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