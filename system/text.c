#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../structs.h"
#include "../def.h"
#include "text.h"

void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* texte, SDL_Color couleur, int x, int y){
    SDL_Surface* surface = TTF_RenderText_Blended(font, texte, couleur);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawButton(SDL_Renderer* renderer, Bouton* bouton){
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
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

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