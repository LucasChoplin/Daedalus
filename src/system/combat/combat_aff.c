#include "combat_aff.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../../structs.h"
#include "../../def.h"
#include <stdlib.h>
#include <stdio.h>
#include "../text.h"
#include "../inventaire.h"

/** \file combat_aff.c
    \brief  contenu des fonction pour afficher l'interface de combat
    \author Lucas Choplin
    \version 1.0
    \date 8 février 2026
*/

void afficherCombat(SDL_Renderer *renderer, Fighter *joueur, Mob ennemi,SDL_Rect fuite,SDL_Rect attack_btn,SDL_Rect forte,SDL_Rect inventaire, int inv, item_t * l[]){
    int largeurBarreFixe = 300;
    SDL_Surface* image;
    switch(joueur->classeID){
        case 0:
            image = SDL_LoadBMP("assets/archer.bmp");
            break;
        case 1:
            image = SDL_LoadBMP("assets/gladiateur.bmp");
            break;
        case 2:
            image = SDL_LoadBMP("assets/lancier.bmp");
            break;
    }
    TTF_Font* font = getCombatFont();

    char puissance[20];
    sprintf(puissance, "Forte %dPM", joueur->pm_atk);

    Bouton boutonAttaquer = {attack_btn, {200, 50, 50, 255}, {255, 255, 255, 255}, "Attaque", font};
    Bouton boutonForte;
    if(joueur->pm_atk>0){
        boutonForte = (Bouton){forte, {50, 50, 200, 255}, {255, 255, 255, 255}, puissance, font};
    }
    else{
        boutonForte = (Bouton){forte, {200, 235, 255, 255}, {255, 255, 255, 255}, puissance, font};
    }
    Bouton boutonFuite = {fuite, {250, 250, 250, 255}, {0, 0, 0, 255}, "Fuite", font};
    Bouton boutonInventaire = {inventaire, {250, 0, 250, 255}, {255, 255, 255, 255}, "Inventaire", font};

    SDL_Texture* monImage = SDL_CreateTextureFromSurface(renderer, image);  

    SDL_RenderCopy(renderer, monImage, NULL, NULL);
    SDL_FreeSurface(image);


    int longueur_xp = (int)((float)joueur->xp / joueur->max_xp * largeurBarreFixe);

    SDL_Rect xp_possede = { 50, 935, longueur_xp, 10 };
    SDL_Rect xp_necessaire = { 50, 935, largeurBarreFixe, 10 };

    SDL_Rect personnage = {150, 800, 0, 0};
    SDL_QueryTexture(monImage, NULL, NULL, &personnage.w, &personnage.h);

    SDL_Rect ennemy = {1100, 100, 0, 0};
    SDL_QueryTexture(monImage, NULL, NULL, &ennemy.w, &ennemy.h);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    //SDL_RenderCopy(renderer, monImage, NULL, NULL);

    int largeurEnnemi = (int)((float)ennemi.hp / ennemi.max_hp * largeurBarreFixe);

    // bare de vie vide 
    SDL_Rect enemy_hp_fond = {1000, 40, largeurBarreFixe, 30};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); 
    SDL_RenderFillRect(renderer, &enemy_hp_fond);

    // bare de vie d'ennemie
    SDL_Rect enemy_hp = {1000, 40, largeurEnnemi, 30};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &enemy_hp);

    char E_hp [40];
    sprintf(E_hp,"%d/%d",ennemi.hp,ennemi.max_hp);

    drawText(renderer,font,E_hp,(SDL_Color){255, 255, 255, 255},1000,40);

    int largeurJoueur = (int)((float)joueur->hp / joueur->max_hp * largeurBarreFixe);

    // Fond de la barre
    SDL_Rect player_hp_fond = {50, 900, largeurBarreFixe, 30};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &player_hp_fond);

    // Partie pleine (verte)
    SDL_Rect player_hp = {50, 900, largeurJoueur, 30};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_hp);


    char P_hp [40];
    sprintf(P_hp,"%d/%d",joueur->hp,joueur->max_hp);
    
    drawText(renderer,font,P_hp,(SDL_Color){255, 255, 255, 255},50,800);

    char P_lvl [40];
    sprintf(P_lvl,"lvl:%d",joueur->lvl);
    
    drawText(renderer,font,P_lvl,(SDL_Color){255, 255, 255, 255},50,825);

    // Bouton Attaquer
    drawButton(renderer, &boutonAttaquer);
    
    // Bouton Attaque forte
    drawButton(renderer, &boutonForte);

    //bouton fuite
    drawButton(renderer, &boutonFuite);
    
    //bouton inventaire
    drawButton(renderer, &boutonInventaire);

    //bouton xp necessaire
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &xp_necessaire);

    //bouton xp
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &xp_possede);

    char P_xp [40];
    sprintf(P_xp,"xp:%d",joueur->xp); 
      
    drawText(renderer,font,P_xp,(SDL_Color){255, 255, 255, 255},50,850);

    SDL_RenderCopy(renderer, monImage, NULL, &personnage);
    SDL_RenderCopy(renderer, monImage, NULL, &ennemy);
    if(inv){        //si inv != 0 afficher l'inventaire 
        combat_afficher_inventaire(renderer,l);
    }

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(monImage);
}

void endScreen(SDL_Renderer *renderer, GameState state, int* x, int* y, TTF_Font* font,loot_t * d){
    SDL_Surface* texte=NULL;
    char* message = NULL; 
    SDL_Color couleur;
    SDL_GetRendererOutputSize(renderer, x, y);
    SDL_Rect position = {0, 0, 0, 0};

    switch(state){
        case DEFAITE:
            couleur = (SDL_Color){200, 0, 0, 255};
            message="DEFEAT";
            break;

        case VICTOIRE:
            couleur = (SDL_Color){0, 200, 0, 255};
            message="VICTORY";
            afficherItemObtenuCombat(renderer,d);
            break;
    }

    if (message) {
        texte = TTF_RenderText_Blended(font, message, couleur);

        if (texte) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, texte);
            SDL_FreeSurface(texte);

            SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
            position.x = (*x - position.w) / 2;
            position.y = (*y - position.h) / 2;

            SDL_RenderCopy(renderer, texture, NULL, &position);
            /*
            if(state==VICTOIRE){
                char xp_text[20];
                char gold_text[20];
                sprintf(xp_text, "XP: %d",xp);

                sprintf(gold_text, "OR: %d",gold);

                SDL_Color blanc = {255, 255, 255, 255};

                drawText(renderer, font, xp_text, blanc, 50, 500);
                drawText(renderer, font, gold_text, blanc, 50, 600);
            }
            */
            SDL_RenderPresent(renderer);

            SDL_Delay(2000);

            SDL_DestroyTexture(texture);
        }
    }

}