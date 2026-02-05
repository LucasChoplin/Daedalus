#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(){
    SDL_Window *window = NULL;  //pt qui representera la fenetre
    SDL_Renderer *renderer = NULL;  //pt pour dessiner
    if(SDL_Init(SDL_INIT_VIDEO)!=0){  //init et alloue
        goto Quit;
    }

    window = SDL_CreateWindow("Daedalus",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                            640, 480, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if(window==NULL || renderer==NULL){         //si échoue pt restera NULL
        goto Quit;
    }
    SDL_Delay(3000);
Quit:   //detruire dans le sens inverse
    if(renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if(window != NULL)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}