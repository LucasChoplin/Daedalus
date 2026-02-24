#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "atlas.h"

static SDL_Texture* atlas = NULL;

//l'atlas comprend toutes les textures possibles de sol/mur
//on ne charge qu'une texture pour toutes les maps

//initialise l'atlas en chargeant la texture
void initAtlas(SDL_Renderer* renderer){
    atlas = IMG_LoadTexture(renderer, "assets/atlas1.png"); //path du sprite de l'atlas
    if(!atlas){
        fprintf(stderr, "Erreur IMG_LoadTexture : %s", SDL_GetError());
        exit(1);
    }
}

//libere la memoire
void cleanupAtlas(void){
    if(atlas){
        SDL_DestroyTexture(atlas);
        atlas = NULL;
    }
}

//pour obtenir la texture de l'atlas
SDL_Texture* getAtlasTexture(void){
    return atlas;
}

//retourne le 'rectange' de l'atlas correspondant à un ID de tile
SDL_Rect getTileRect(int ID){
    SDL_Rect rect;
    rect.x = (ID % ATLAS_COLUMNS) * TILE_SIZE;
    rect.y = (ID / ATLAS_COLUMNS) * TILE_SIZE;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
    return rect;
}