#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "structs.h"
#include "system/map.h"
#include "system/atlas.h"
#include "system/text.h"
#include "def.h"


Game game;
Player player;
Mob mobTest = {.mapID=1, .xTile=5, .yTile=5};
Bouton btnF = {.couleurFond.r = 80, .couleurFond.g = 80, .couleurFond.b = 80, .couleurFond.a = 255, .couleurTexte.r = 255, .couleurTexte.g = 255, .couleurTexte.b = 255, .couleurTexte.a = 255, .texte = "F"};

int initSDL(void){
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }

    game.window = SDL_CreateWindow("Daedalus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if(!game.window){
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        return -1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    game.renderer = SDL_CreateRenderer(game.window, -1, rendererFlags);
    if(!game.renderer){
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        return -1;
    }

    if(TTF_Init() < 0){
        fprintf(stderr, "Erreur TTF_Init : %s", TTF_GetError());
        return -1;
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    
    initAtlas(game.renderer);
    initMobAtlas(game.renderer);

    return 0;
}

////////////////////drawing///////////////////////  
void prepareScene(void){
    SDL_SetRenderDrawColor(game.renderer, 159, 139, 122, 255);
    SDL_RenderClear(game.renderer);
}

//charge une img et retourne une texture
SDL_Texture* loadTexture(char * filename){
    SDL_Texture* texture;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    texture = IMG_LoadTexture(game.renderer, filename);

    return texture;
}

//affiche une texture a une position et dimension donnee
void drawTexture(SDL_Texture* texture, int x, int y, int w, int h){
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    SDL_RenderCopy(game.renderer, texture, NULL, &dest);
}

//evite collision avec les mobs
int checkMobCollision(int x, int y) {
    if (currentMap->mapID == mobTest.mapID) {
        if (mobTest.xTile == x && mobTest.yTile == y) {
            return 1; //si mob
        }
    }
    return 0; 
}

//nettoyage de la memoire
void cleanup(void){
    cleanupAtlas();
    cleanupMobAtlas();
    cleanupMap();
    if(game.renderer != NULL)
        SDL_DestroyRenderer(game.renderer);
    if(game.window != NULL)
        SDL_DestroyWindow(game.window);
    TTF_Quit();
    SDL_Quit();
}

int main(){
    //initialise les pointeurs a NULL
    memset(&game, 0, sizeof(Game)); 
    memset(&player, 0, sizeof(Player));

    initSDL();
    TTF_Font* font = TTF_OpenFont("assets/DejaVuSans.ttf", 24);

    //apres l'init de TTF, on fini d'init le bouton d'interaction
    btnF.font = font;
    btnF.rect.w = 30;  //bouton carre
    btnF.rect.h = 30;

    int mapPixelWidth = SALLE_WIDTH * TILE_SIZE;
    int mapPixelHeight = SALLE_HEIGHT * TILE_SIZE;
    int mapOffsetX = (SCREEN_WIDTH - mapPixelWidth) / 2;
    int mapOffsetY = (SCREEN_HEIGHT - mapPixelHeight) / 2;

    initAtlas(game.renderer);
    initMap();

    //position de depart du player
    player.xTile = 9;
    player.yTile = 7;
    player.texture = loadTexture("assets/glad.png"); //chemin du sprite du player
    atexit(cleanup); //nettoyera la memoire à la fermeture du programme
    ////////////////////////
    
    while(1){
        prepareScene();
        SDL_Event e;
        
        //gestion des evenements
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                exit(0);
            }
            //si joueur a cote du mob
            if(currentMap->mapID == mobTest.mapID) {
                if ((abs(player.xTile - mobTest.xTile) + abs(player.yTile - mobTest.yTile)) == 1) {
                    //si appuie sur F, lance combat
                    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){
                        printf("Combat!\n");
                    }
                }
            }

            if(e.type == SDL_KEYDOWN){
                int newX = player.xTile;
                int newY = player.yTile;

                switch(e.key.keysym.sym){
                    case SDLK_UP:
                        newY --;
                        break;
                    case SDLK_DOWN:
                        newY ++;
                        break;
                    case SDLK_LEFT:
                        newX --;
                        break;
                    case SDLK_RIGHT:
                        newX ++;
                        break;
                }
                //on verifie les collisions et le changement de salle
                if(newX >= SALLE_WIDTH) {
                    changeRoom(DROITE, &newX, &newY);
                    int tile = currentMap->tiles[newY][newX];
                    if(!isWall(tile) && !checkMobCollision(newX, newY)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                } else if(newX < 0) {
                    changeRoom(GAUCHE, &newX, &newY);
                    int tile = currentMap->tiles[newY][newX];
                    if(!isWall(tile) && !checkMobCollision(newX, newY)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                } else if(newY >= SALLE_HEIGHT) {
                    changeRoom(BAS, &newX, &newY);
                    int tile = currentMap->tiles[newY][newX];
                    if(!isWall(tile) && !checkMobCollision(newX, newY)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                } else if(newY < 0) {
                    changeRoom(HAUT, &newX, &newY);
                    int tile = currentMap->tiles[newY][newX];
                    if(!isWall(tile) && !checkMobCollision(newX, newY)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                } else {
                    int tile = currentMap->tiles[newY][newX];
                    if(!isWall(tile) && !checkMobCollision(newX, newY)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                }
            }
        }

        drawMap(game.renderer);
        if (currentMap->mapID == mobTest.mapID) {
            drawMob(game.renderer, &mobTest);
        }
        drawTexture(player.texture, player.xTile * TILE_SIZE, player.yTile * TILE_SIZE, 64, 64);
        //dessine le bouton si a cote du mob
        if (currentMap->mapID == mobTest.mapID &&
            (abs(player.xTile - mobTest.xTile) + abs(player.yTile - mobTest.yTile)) == 1){
            btnF.rect.x = player.xTile * TILE_SIZE + 50;
            btnF.rect.y = player.yTile * TILE_SIZE - 20;
            drawButton(game.renderer, &btnF);
        }
        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }

    return 0;
}