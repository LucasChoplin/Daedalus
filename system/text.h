#ifndef text_h //evite les inclusions multiples
#define text_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../structs.h"
/** \file text.h
    \brief librairie pour l'affichage de text 
    \author Myriam Laaqira
    \version 1.0
    \date mars ??
*/

int initText(void);
void cleanupText(void);
TTF_Font* getDefaultFont(void);
TTF_Font* getTitleFont(void);
TTF_Font* getCombatFont(void);
TTF_Font* getEndScreenFont(void);
void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* texte, SDL_Color couleur, int x, int y);
void drawButton(SDL_Renderer* renderer, Bouton* bouton);
/** \brief fonction qui écrit des chiffres
    \param r pointeur vers le moteur de rendu 
    \param nb nombre à afficher 
    \param x paramètre x des coordoonées où afficher le nombre 
    \param y paramètre y des coordoonées où afficher le nombre 
*/
void drawChiffre(SDL_Renderer * r,int nb, int x, int y);
#endif