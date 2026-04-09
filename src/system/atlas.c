#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "atlas.h"

/** \file atlas.c
    \brief contenus des fonctions de atlas.h
    \author Myriam Laaqira
    \version 1.0
    \date 12 février 2026
*/

static SDL_Texture* atlas = NULL;
static SDL_Texture* mobAtlas = NULL;
static SDL_Texture * atlasItem = NULL;//texture qui sert à stocker l'atlas des items 
static SDL_Texture * atlasMenu = NULL;//texture qui sert à stocker l'atlas des menus 
static SDL_Texture * atlasPerso = NULL;//texture qui sert à stocker l'atlas des persos



//l'atlas comprend toutes les textures possibles de sol/mur
//on ne charge qu'une texture pour toutes les maps
//on a aussi un atlas pour les diff mobs 

//initialise l'atlas en chargeant la texture
void initAtlasMap(SDL_Renderer* r){
    atlas = IMG_LoadTexture(r, "assets/atlas_etage1.png"); // path du sprite de l'atlas
    if(!atlas){
        fprintf(stderr, "Erreur IMG_LoadTexture (map) : %s", SDL_GetError());
        exit(1);
    }
}

void initAtlasMob(SDL_Renderer* r){
    mobAtlas = IMG_LoadTexture(r, "assets/atlas_mob.png");
    if(!mobAtlas){
        fprintf(stderr, "Erreur IMG_LoadTexture (mob) : %s", SDL_GetError());
        exit(1);
    }
    SDL_SetTextureScaleMode(mobAtlas, SDL_ScaleModeNearest);
}

void initAtlasItem(SDL_Renderer * r){
    atlasItem = IMG_LoadTexture(r, "assets/items.png");
    if(!atlasItem){
        fprintf(stderr, "Erreur IMG_LoadTexture (atlasItem) : %s", SDL_GetError());
        exit(1);
    }
    SDL_SetTextureScaleMode(atlasItem, SDL_ScaleModeNearest);
}

void initAtlasMenu(SDL_Renderer * r){
    atlasMenu = IMG_LoadTexture(r, "assets/bouton.png");
    if(!atlasMenu){
        fprintf(stderr, "Erreur IMG_LoadTexture (atlasMenu) : %s", SDL_GetError());
        exit(1);
    }
    //evite le bleeding entre tuiles adjacentes (petit trait visible)
    SDL_SetTextureScaleMode(atlasMenu, SDL_ScaleModeNearest);
}

void initAtlasPerso(SDL_Renderer * r){
    atlasPerso = IMG_LoadTexture(r, "assets/perso.png");
    if(!atlasPerso){
        fprintf(stderr, "Erreur IMG_LoadTexture (atlasPerso) : %s", SDL_GetError());
        exit(1);
    }
    SDL_SetTextureScaleMode(atlasPerso, SDL_ScaleModeNearest);
}

void cleanupAtlasMap(void){
    if(atlas){
        SDL_DestroyTexture(atlas);
        atlas = NULL;
    }
}

void cleanupAtlasMob(void){
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
SDL_Texture* getAtlasMap(void){
    return atlas;
}

SDL_Texture* getAtlasMob(void){
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
SDL_Rect getTileRect(int ID, int tailleAtlas){
    SDL_Rect rect;
    rect.x = (ID % tailleAtlas) * TAILLE_TUILE;
    rect.y = (ID / tailleAtlas) * TAILLE_TUILE;
    rect.w = TAILLE_TUILE;
    rect.h = TAILLE_TUILE;
    return rect;
}

SDL_Rect getItemRect(int ID){
    SDL_Rect rect;
    rect.x = (ID % ATLAS_ITEM) * TAILLE_ITEM;
    rect.y = (ID / ATLAS_ITEM) * TAILLE_ITEM;
    rect.w = TAILLE_ITEM;
    rect.h = TAILLE_ITEM;
    return rect;
}