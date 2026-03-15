#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "map.h"
#include "atlas.h"
#include "../def.h"


// 0-3 = sol milieu, 3-6 = sol bord, 8 = mur bas, 9-10 = mur haut

Salle* currentMap = NULL;
Salle* etage[ETAGE_WIDTH * ETAGE_HEIGHT];

//genere salle normale avec murs en bords et sols
void genererSalle(Salle* salle){
    for(int y = 0; y < SALLE_HEIGHT; y++){
        for(int x = 0; x < SALLE_WIDTH; x++){
            if(y == 0){
                // mur haut
                //salle->tiles[y][x] = 9 + rand() % 2;
                salle->tiles[y][x] = 11;
            } else if(y == SALLE_HEIGHT - 2){
                // mur bas
                salle->tiles[y][x] = 9 + rand() % 2;
            } else if(y == SALLE_HEIGHT - 1){
                // mur bas
                salle->tiles[y][x] = 8;
            } else if(x == 0 || x == SALLE_WIDTH - 1){
                // murs gauche et droite
                salle->tiles[y][x] = 11;
            } else {
                // sol
                if(y == 1){
                    // sol relié au mur du haut
                    salle->tiles[y][x] = 4 + rand() % 3;
                } else {
                    // sol milieu
                    salle->tiles[y][x] = rand() % 4;
                }
            }
        }
    }
}

void connecterSalles(Salle* salle1, Salle* salle2, int direction){
    int x1 = SALLE_WIDTH / 2;
    int y1 = SALLE_HEIGHT / 2;
    //en fct de la direction on creuse un passage pour les salles
    switch(direction){
        case DROITE:
            for(int i = -1; i <= 1; i++) {
                salle1->tiles[y1 + i][SALLE_WIDTH - 1] = 0;
                salle2->tiles[y1 + i][0] = 0;
            }
            break;
        case BAS:
            for(int i = -1; i <= 1; i++) {
                salle1->tiles[SALLE_HEIGHT - 1][x1 + i] = 0; 
                salle2->tiles[0][x1 + i] = 0; 
            }
            break;
        case GAUCHE:
            for(int i = -1; i <= 1; i++) {
                salle1->tiles[y1 + i][0] = 0; 
                salle2->tiles[y1 + i][SALLE_WIDTH - 1] = 0; 
            }
            break;
        case HAUT:
            for(int i = -1; i <= 1; i++) {
                salle1->tiles[0][x1 + i] = 0;
                salle2->tiles[SALLE_HEIGHT - 1][x1 + i] = 0;
            }
            break;
    }
}

//verifie si tile est un mur ou pas
int isWall(int tile){
    return (tile >= 6); 
}

void initMap(void) {
    srand(time(NULL));
    //creation des salles pour l'etage
    for(int i = 0; i < ETAGE_WIDTH * ETAGE_HEIGHT; i++){
        etage[i] = malloc(sizeof(Salle));
        etage[i]->mapID = i;
        etage[i]->xSalle = i % ETAGE_WIDTH;
        etage[i]->ySalle = i / ETAGE_WIDTH;
        genererSalle(etage[i]);
    }
    
    //puis connexion des salles
    for(int y = 0; y < ETAGE_HEIGHT; y++){
        for(int x = 0; x < ETAGE_WIDTH; x++) {
            int idx = y * ETAGE_WIDTH + x;
            
            //connexion horizontale
            if(x < ETAGE_WIDTH - 1 && rand() % 2) { //choix aleatoire
                connecterSalles(etage[idx], etage[idx + 1], DROITE); //on fait ici 1 direction pour eviter de connecter 2 fois les memes salles
            }
            
            //connexion verticale
            if(y < ETAGE_HEIGHT - 1 && rand() % 2) {
                connecterSalles(etage[idx], etage[idx + ETAGE_WIDTH], BAS);
            }
        }
    }
    
    //le spawn est la salle 0
    currentMap = etage[0];
}

//affiche la map et la centre a l'ecran
void drawMap(SDL_Renderer* renderer){
    if(!currentMap) return;
    
    SDL_Rect src;
    SDL_Texture* atlas = getAtlasTexture();
    for(int y = 0; y < SALLE_HEIGHT; y++){ //parcours de la map
        for(int x = 0; x < SALLE_WIDTH; x++){ 
            int tileID = currentMap->tiles[y][x];
            src = getTileRect(tileID, ATLAS_MAP);

            SDL_Rect destRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            SDL_RenderCopy(renderer, atlas, &src, &destRect);
        }
    }
}

void drawMob(SDL_Renderer* renderer, Mob* mob) {
    if(!mob) return;
    
    SDL_Rect src = getTileRect(0, ATLAS_PERSO);
    SDL_Texture* mobAtlas = getMobAtlasTexture();
    
    SDL_Rect destRect = {mob->xTile * TILE_SIZE, mob->yTile * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, mobAtlas, &src, &destRect);
}

void cleanupMap(void) {
    for(int i = 0; i < ETAGE_WIDTH * ETAGE_HEIGHT; i++) {
        if(etage[i]) {
            free(etage[i]);
            etage[i] = NULL;
        }
    }
    currentMap = NULL;
}

void changeSalle(int direction, int* playerX, int* playerY) {
    int idNouvelleSalle;
    int currentX = currentMap->xSalle;
    int currentY = currentMap->ySalle;
    int newX = currentX;
    int newY = currentY;
    
    //change la position du joueur
    switch(direction) {
        case DROITE:
            newX = currentX + 1;
            *playerX = 0;
            *playerY = SALLE_HEIGHT / 2;
            break;
        case BAS: 
            newY = currentY + 1;
            *playerX = SALLE_WIDTH / 2;
            *playerY = 0;
            break;
        case GAUCHE:
            newX = currentX - 1;
            *playerX = SALLE_WIDTH - 1;
            *playerY = SALLE_HEIGHT / 2;
            break;
        case HAUT:
            newY = currentY - 1;
            *playerX = SALLE_WIDTH / 2;
            *playerY = SALLE_HEIGHT - 1;
            break;
    }
    
    // Vérification des limites
    if(newX < 0 || newX >= ETAGE_WIDTH || newY < 0 || newY >= ETAGE_HEIGHT) {
        return; // Pas de salle en dehors de la grille
    }
    
    //on change la salle actuelle
    idNouvelleSalle = newY * ETAGE_WIDTH + newX;
    currentMap = etage[idNouvelleSalle];
}

void changeRoom(int direction, int* playerX, int* playerY) {
    changeSalle(direction, playerX, playerY);
}
