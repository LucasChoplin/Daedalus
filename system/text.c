#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../structs.h"
#include "../def.h"
#include "text.h"

static TTF_Font* defaultFont = NULL;
static TTF_Font* titleFont = NULL;
static TTF_Font* combatFont = NULL;
static TTF_Font* endScreenFont = NULL;

int initText(void){
    defaultFont = TTF_OpenFont("assets/DejaVuSans.ttf", 24);
    titleFont = TTF_OpenFont("assets/LABYRINT.TTF", 72);
    combatFont = TTF_OpenFont("assets/DejaVuSans.ttf", 22);
    endScreenFont = TTF_OpenFont("assets/DejaVuSans.ttf", 100);

    if(defaultFont == NULL || titleFont == NULL || combatFont == NULL || endScreenFont == NULL){
        fprintf(stderr, "Erreur chargement fonts : %s\n", TTF_GetError());
        cleanupText();
        return -1;
    }

    return 0;
}

void cleanupText(void){
    if(defaultFont != NULL){
        TTF_CloseFont(defaultFont);
        defaultFont = NULL;
    }
    if(titleFont != NULL){
        TTF_CloseFont(titleFont);
        titleFont = NULL;
    }
    if(combatFont != NULL){
        TTF_CloseFont(combatFont);
        combatFont = NULL;
    }
    if(endScreenFont != NULL){
        TTF_CloseFont(endScreenFont);
        endScreenFont = NULL;
    }
}

TTF_Font* getDefaultFont(void){
    return defaultFont;
}

TTF_Font* getTitleFont(void){
    return titleFont;
}

TTF_Font* getCombatFont(void){
    return combatFont;
}

TTF_Font* getEndScreenFont(void){
    return endScreenFont;
}

void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* texte, SDL_Color couleur, int x, int y){
    if(font == NULL){
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, texte, couleur);
    if(surface == NULL){
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL){
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dest = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawButton(SDL_Renderer* renderer, Bouton* bouton){
    if(bouton == NULL || bouton->font == NULL){
        return;
    }

    //on dessine le fond
    SDL_SetRenderDrawColor(renderer, bouton->couleurFond.r, bouton->couleurFond.g, bouton->couleurFond.b, bouton->couleurFond.a);
    SDL_RenderFillRect(renderer, &bouton->rect);

    //et la bordure
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  //bord gris 
    SDL_Rect borderRect = bouton->rect;
    for (int i = 0; i < 3; i++) {
        SDL_RenderDrawRect(renderer, &borderRect);
        borderRect.x--;
        borderRect.y--;
        borderRect.w += 2;
        borderRect.h += 2;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(bouton->font, bouton->texte, bouton->couleurTexte);
    if(surface == NULL){
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL){
        SDL_FreeSurface(surface);
        return;
    }

    //on centre le texte dans le bouton
    SDL_Rect rectTexte;
    rectTexte.w = surface->w;
    rectTexte.h = surface->h;
    rectTexte.x = bouton->rect.x + (bouton->rect.w - rectTexte.w) / 2;
    rectTexte.y = bouton->rect.y + (bouton->rect.h - rectTexte.h) / 2;
    SDL_RenderCopy(renderer, texture, NULL, &rectTexte);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}