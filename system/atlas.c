#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "atlas.h"

static SDL_Texture* atlas = NULL;
static SDL_Texture* mobAtlas = NULL;
static SDL_Texture * atlasItem = NULL;//texture qui sert à stocker l'atlas des items 
static SDL_Texture * atlasMenu = NULL;//texture qui sert à stocker l'atlas des menus 
static SDL_Texture * atlasPerso = NULL;//texture qui sert à stocker l'atlas des persos



//l'atlas comprend toutes les textures possibles de sol/mur
//on ne charge qu'une texture pour toutes les maps
//on a aussi un atlas pour les diff mobs 

//initialise l'atlas en chargeant la texture
void initAtlas(SDL_Renderer* renderer){
    atlas = IMG_LoadTexture(renderer, "assets/atlas_b1.png"); // path du sprite de l'atlas
    if(!atlas){
        fprintf(stderr, "Erreur IMG_LoadTexture (map) : %s", SDL_GetError());
        exit(1);
    }
}

void initMobAtlas(SDL_Renderer* renderer){
    mobAtlas = IMG_LoadTexture(renderer, "assets/jokere.png");
    if(!mobAtlas){
        fprintf(stderr, "Erreur IMG_LoadTexture (mob) : %s", SDL_GetError());
        exit(1);
    }
}

void initAtlasItem(SDL_Renderer * r){
    atlasItem = IMG_LoadTexture(r, "assets/Item.png");
    if(!atlasItem){
        fprintf(stderr, "Erreur IMG_LoadTexture (atlasItem) : %s", SDL_GetError());
        exit(1);
    }
}

void initAtlasMenu(SDL_Renderer * r){
    atlasMenu = IMG_LoadTexture(r, "assets/bouton.png");
    if(!atlasMenu){
        fprintf(stderr, "Erreur IMG_LoadTexture (atlasMenu) : %s", SDL_GetError());
        exit(1);
    }
}

void initAtlasPerso(SDL_Renderer * r){
    atlasPerso = IMG_LoadTexture(r, "assets/perso.png");
    if(!atlasPerso){
        fprintf(stderr, "Erreur IMG_LoadTexture (atlasItem) : %s", SDL_GetError());
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

void cleanupMobAtlas(void){
    if(mobAtlas){
        SDL_DestroyTexture(mobAtlas);
        mobAtlas = NULL;
    }
}

void cleanupAtlasItem(void){
    if(atlasItem){
        SDL_DestroyTexture(atlasItem);
        atlasItem = NULL;
    }
}

void cleanupAtlasMenu(void){
    if(atlasMenu){
        SDL_DestroyTexture(atlasMenu);
        atlasMenu = NULL;
    }
}

void cleanupAtlasPerso(void){
    if(atlasPerso){
        SDL_DestroyTexture(atlasPerso);
        atlasPerso = NULL;
    }
}

//pour obtenir la texture de l'atlas
SDL_Texture* getAtlasTexture(void){
    return atlas;
}

SDL_Texture* getMobAtlasTexture(void){
    return mobAtlas;
}

SDL_Texture* getAtlasItem(void){
    return atlasItem;
}

SDL_Texture * getAtlasMenu(void){
    return atlasMenu;
}

SDL_Texture * getAtlasPerso(void){
    return atlasPerso;
}

//retourne le 'rectange' de l'atlas correspondant à un ID de tile

SDL_Rect getTileRect(int ID,int TAILLE_ATLAS){
    SDL_Rect rect;
    rect.x = (ID % TAILLE_ATLAS) * TILE_SIZE;
    rect.y = (ID / TAILLE_ATLAS) * TILE_SIZE;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
    return rect;
}