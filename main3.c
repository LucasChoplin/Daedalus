#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"
#include "system/map.h"
#include "system/atlas.h"
#include "def.h"


Game game;
Player player;
Mob mobTest = {.mapID=1, .tileX=5, .tileY=5};

//evite collision avec les mobs
/*int checkMobCollision(int x, int y) {
    if (currentMap->mapID == mobTest.mapID) {
        if (mobTest.tileX == x && mobTest.tileY == y) {
            return 1; //si mob
        }
    }
    return 0; 
}*/

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

//nettoyage de la memoire
void cleanup(void){
    cleanupAtlas();
    cleanupMobAtlas();
    cleanupMap();
    if(game.renderer != NULL)
        SDL_DestroyRenderer(game.renderer);
    if(game.window != NULL)
        SDL_DestroyWindow(game.window);
    SDL_Quit();
}

int main(){
    //initialise les pointeurs a NULL
    memset(&game, 0, sizeof(Game)); 
    memset(&player, 0, sizeof(Player));

    initSDL();

    int mapPixelWidth = SALLE_WIDTH * TILE_SIZE;
    int mapPixelHeight = SALLE_HEIGHT * TILE_SIZE;
    int mapOffsetX = (SCREEN_WIDTH - mapPixelWidth) / 2;
    int mapOffsetY = (SCREEN_HEIGHT - mapPixelHeight) / 2;

    initAtlas(game.renderer);
    initMap();

    //position de depart du player
    player.xTile = 9;
    player.yTile = 7;
    player.texture = loadTexture("assets/batman.png"); //chemin du sprite du player
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
                    if(!isWall(tile)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                } else if(newX < 0) {
                    changeRoom(GAUCHE, &newX, &newY);
                    int tile = currentMap->tiles[newY][newX];
                    if(!isWall(tile)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                } else if(newY >= SALLE_HEIGHT) {
                    changeRoom(BAS, &newX, &newY);
                    int tile = currentMap->tiles[newY][newX];
                    if(!isWall(tile)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                } else if(newY < 0) {
                    changeRoom(HAUT, &newX, &newY);
                    int tile = currentMap->tiles[newY][newX];
                    if(!isWall(tile)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                } else {
                    int tile = currentMap->tiles[newY][newX];
                    if(!isWall(tile)){
                        player.xTile = newX;
                        player.yTile = newY;
                    }
                }
            }
        }
        if (player.xTile == mobTest.tileX && player.yTile == mobTest.tileY && currentMap->mapID == mobTest.mapID) {
            //fonction declenchement combat;
            printf("Combat!\n");
        }
        drawMap(game.renderer);
        if (currentMap->mapID == mobTest.mapID) {
            drawMob(game.renderer, &mobTest);
        }
        drawTexture(player.texture, player.xTile * TILE_SIZE, player.yTile * TILE_SIZE, 64, 64);
        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }

    return 0;
}