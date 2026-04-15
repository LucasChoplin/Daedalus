#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/structs.h"
#include "../lib/def.h"
#include "../lib/text.h"

/** \file text.c
    \brief contenu des fonctions de text.h 
    \author Myriam Laaqira
    \version 1.0
    \date 11 mars 2026
*/

static TTF_Font* defaultFont = NULL;
static TTF_Font* titleFont = NULL;
static TTF_Font* combatFont = NULL;
static TTF_Font* endScreenFont = NULL;
static TTF_Font* classNameFont = NULL;
static TTF_Font* classStatsFont = NULL;
static TTF_Font* menuFont = NULL;
static TTF_Font* xpFont = NULL;

int initText(void){
    defaultFont = TTF_OpenFont("assets/DejaVuSans.ttf", 24);
    titleFont = TTF_OpenFont("assets/LABYRINT.TTF", 110);
    combatFont = TTF_OpenFont("assets/DejaVuSans.ttf", 22);
    endScreenFont = TTF_OpenFont("assets/DejaVuSans.ttf", 100);
    classNameFont = TTF_OpenFont("assets/LABYRINT.TTF", 26);
    classStatsFont = TTF_OpenFont("assets/DejaVuSans.ttf", 24);
    menuFont = TTF_OpenFont("assets/DejaVuSans.ttf", 44);
    xpFont = TTF_OpenFont("assets/LABYRINT.TTF", 65);

    if(defaultFont == NULL || titleFont == NULL || combatFont == NULL || endScreenFont == NULL || classNameFont == NULL || classStatsFont == NULL || menuFont == NULL){
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
    if(classNameFont != NULL){
        TTF_CloseFont(classNameFont);
        classNameFont = NULL;
    }
    if(classStatsFont != NULL){
        TTF_CloseFont(classStatsFont);
        classStatsFont = NULL;
    }
    if(menuFont != NULL){
        TTF_CloseFont(menuFont);
        menuFont = NULL;
    }
    if(xpFont != NULL){
        TTF_CloseFont(xpFont);
        xpFont = NULL;
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

TTF_Font* getClassNameFont(void){
    return classNameFont;
}

TTF_Font* getClassStatsFont(void){
    return classStatsFont;
}

TTF_Font* getMenuFont(void){
    return menuFont;
}

TTF_Font* getXpFont(void){
    return xpFont;
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

void drawChiffre(SDL_Renderer * r,int nb, int x, int y){
    char chiffre[10];
    chiffre[0]='\0';
    if(nb == 0){//cas où le nombre est 0
        chiffre[0] = '0';
        chiffre[1] = '\0';
    }
    for(int j =1;j-1<nb;j*=10){/* pour transformer les nombres en chaînes de caractères */
        for(int k=9;k>0;k--){
            chiffre[k] = chiffre[k-1];
        }
        chiffre[0] = '0' + (nb%(j*10)/j) ;
    }
    TTF_Font* titleFont = getMenuFont();
    SDL_Texture* titleTexture = NULL;
    SDL_Rect titleRect = {0, 0, 0, 0};
    SDL_Color titleColor = {240, 230, 180, 255};
    SDL_Surface* titleSurface = TTF_RenderUTF8_Blended(titleFont, chiffre, titleColor);
    titleTexture = SDL_CreateTextureFromSurface(game.renderer, titleSurface);
    titleRect.w = titleSurface->w;
    titleRect.h = titleSurface->h;
    titleRect.x = x;
    titleRect.y = y;
    SDL_FreeSurface(titleSurface);
    SDL_RenderCopy(r,titleTexture,NULL,&titleRect);
    SDL_DestroyTexture(titleTexture);
}