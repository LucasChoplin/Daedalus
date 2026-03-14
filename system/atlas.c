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
    atlas = IMG_LoadTexture(renderer, "assets/atlas1.png"); // path du sprite de l'atlas
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
    //chargerImage("Img/potion.bmp",r,&atlasItem);
}

void initAtlasMenu(SDL_Renderer * r){
    atlasMenu = IMG_LoadTexture(r, "assets/bouton.png");
}

void initAtlasPerso(SDL_Renderer * r){
    atlasPerso = IMG_LoadTexture(r, "assets/perso.png");
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
    SDL_DestroyTexture(atlasItem);
}

void cleanupAtlasMenu(void){
    SDL_DestroyTexture(atlasMenu);
}

void cleanupAtlasPerso(void){
    SDL_DestroyTexture(atlasPerso);
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
SDL_Rect getTileRect(int ID){
    SDL_Rect rect;
    rect.x = (ID % ATLAS_COLUMNS) * TILE_SIZE;
    rect.y = (ID / ATLAS_COLUMNS) * TILE_SIZE;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
    return rect;
}

SDL_Rect getTileRect2(int ID,int TAILLE_ATLAS){
    SDL_Rect rect;
    rect.x = (ID % TAILLE_ATLAS) * 64;
    rect.y = (ID / TAILLE_ATLAS) * 64;
    rect.w = 64;
    rect.h = 64;
    return rect;
}