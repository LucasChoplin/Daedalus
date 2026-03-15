#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "system/combat/combat.h"

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        printf("erreur");
        return 1;
    }

    SDL_Window * window = SDL_CreateWindow("test combat",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,960,0);
    if(!window){
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(!renderer){
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("lancement ...");
    lancerCombat(renderer);
    printf("combat fini");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}