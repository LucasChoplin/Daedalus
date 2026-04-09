#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "inventaire.h"
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

int detecterSourisDansRect(int x, int y, SDL_Rect * r){
    SDL_Point p = {x,y}; 
    return 0;
    return SDL_PointInRect(&p,r); 
}

void afficherChiffre(SDL_Renderer * r,SDL_Texture * t,int nb,SDL_Rect * d){
    d->w = 32;
    d->h = 32;
    int decalage = 0;
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
        decalage += 32;
    }
    d->x -= decalage;
    SDL_RenderCopy(r, t,&chif,d);
}

void saveGameData(Fighter *fighter, item_t *listeItem[], int etageActuel){
    FILE *f = fopen(FICHIER_DATA, "w");
    if(!f){
        return;
    }

    fprintf(f,"classeID=%d\n",fighter->classeID);
    fprintf(f,"pv_max=%d\n",fighter->max_hp);
    fprintf(f,"pv=%d\n",fighter->hp);
    fprintf(f,"stat_attaque=%d\n",fighter->attack);
    fprintf(f,"stat_speed=%d\n",fighter->speed);
    fprintf(f,"xp=%d\n",fighter->xp);
    fprintf(f,"xp_max=%d\n",fighter->max_xp);
    fprintf(f,"niveau=%d\n",fighter->lvl);
    fprintf(f,"gold=%d\n",fighter->gold);
    fprintf(f,"nb_potions=%d\n",listeItem[0]->nb);
    fprintf(f,"nb_superpotions=%d\n",listeItem[1]->nb);
    fprintf(f,"nb_PotionEnergie=%d\n",listeItem[2]->nb);
    fprintf(f,"nb_clés=%d\n",listeItem[3]->nb);
    fprintf(f,"nb_corne=%d\n",listeItem[4]->nb);
    fprintf(f,"nb_anneau=%d\n",listeItem[5]->nb);
    fprintf(f,"nb_sabot=%d\n",listeItem[6]->nb);
    fprintf(f,"nb_couponReduction=%d\n",listeItem[7]->nb);
    fprintf(f,"etage=%d\n",etageActuel);
    fclose(f);
}

void chargerDonnées(Fighter *fighter, item_t *listeItem[], int *etageActuel){
    FILE * f = fopen(FICHIER_DATA,"r");//chargement des données dans les variables locales 
    fscanf(f,"classeID=%d\n",&((*fighter).classeID));
    fscanf(f,"pv_max=%d\n",&((*fighter).max_hp));
    fscanf(f,"pv=%d\n",&((*fighter).hp));
    fscanf(f,"stat_attaque=%d\n",&((*fighter).attack));
    fscanf(f,"stat_speed=%d\n",&((*fighter).speed));
    fscanf(f,"xp=%d\n",&((*fighter).xp));
    fscanf(f,"xp_max=%d\n",&((*fighter).max_xp));
    fscanf(f,"niveau=%d\n",&((*fighter).lvl));
    fscanf(f,"gold=%d\n",&((*fighter).gold));
    fscanf(f,"nb_potions=%d\n",&((*listeItem[0]).nb));
    fscanf(f,"nb_superpotions=%d\n",&((*listeItem[1]).nb));
    fscanf(f,"nb_PotionEnergie=%d\n",&((*listeItem[2]).nb));
    fscanf(f,"nb_clés=%d\n",&((*listeItem[3]).nb));
    fscanf(f,"nb_corne=%d\n",&((*listeItem[4]).nb));
    fscanf(f,"nb_anneau=%d\n",&((*listeItem[5]).nb));
    fscanf(f,"nb_sabot=%d\n",&((*listeItem[6]).nb));
    fscanf(f,"nb_couponReduction=%d\n",&((*listeItem[7]).nb));
    if(fscanf(f,"etage=%d\n",&((*etageActuel))) != 1){
        *etageActuel = 1;
    }
    if(*etageActuel < 1){
        *etageActuel = 1;
    }
    fclose(f);  
}

void defaite(Fighter * p, item_t * l[], int * etageActuel, int * coffre, loot_t * stockMarchand){
    chargerDonnées(p,l,etageActuel);
    p->hp = p->max_hp;
    *coffre = 0;
    *stockMarchand = initStockMarchand();
}