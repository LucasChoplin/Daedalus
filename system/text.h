#ifndef text_h //evite les inclusions multiples
#define text_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../structs.h"

int initText(void);
void cleanupText(void);
TTF_Font* getDefaultFont(void);
TTF_Font* getTitleFont(void);
TTF_Font* getCombatFont(void);
TTF_Font* getEndScreenFont(void);
void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* texte, SDL_Color couleur, int x, int y);
void drawButton(SDL_Renderer* renderer, Bouton* bouton);
#endif