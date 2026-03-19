#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "map.h"
#include "atlas.h"
#include "../def.h"
/** \file map.c
    \brief contenus des fonctions de map.h 
    \author Myriam Laaqira
    \version 1.0
    \date février ??
*/

// 0-1 = sol normal, 2 = sol combat, 3-4 = sol spe, 5-7 = sol bord normal, 8-9 = sol bord combat, 10 = sol bord spe, 
//11-13 = mur bas normal, 14 = vide bas, 15-17 = mur gauche, 18 = mur haut, 19-20 = mur haut spe, 21-23 = mur droite

Salle* currentMap = NULL;
Salle* etage[ETAGE_TAILLE * ETAGE_TAILLE];

//liste des salles reellement presentes dans l'etage
static int listeActives[NB_SALLES_ACTIVES]; 
static int nbrSallesActives = 0;

int getIDSalleRandom(void){ //pour positionner le mini boss dans une salle random et mettre le spawn
    if(nbrSallesActives == 0) return 0;
    return listeActives[rand() % nbrSallesActives];
}

//init la seed du random 
static void initRandom(void){ 
    static int randomInit = 0;

    if(!randomInit){
        srand(time(NULL));
        randomInit = 1;
    }
}

//recup l'id de la salle voisine a partir d'une direction donnee
static int getIDSalleVoisine(int idSalle, int direction){ 
    int x = idSalle % ETAGE_TAILLE;
    int y = idSalle / ETAGE_TAILLE;

    switch(direction){
        case DROITE:
            x++;
            break;
        case BAS:
            y++;
            break;
        case GAUCHE:
            x--;
            break;
        case HAUT:
            y--;
            break;
    }

    //cas si on sort de l'etage
    if(x < 0 || x >= ETAGE_TAILLE || y < 0 || y >= ETAGE_TAILLE){
        return -1;
    }

    return y * ETAGE_TAILLE + x;
}

//genere salle normale avec murs en bords et sols
void genererSalle(Salle* salle){
    for(int y = 0; y < SALLE_HEIGHT; y++){
        for(int x = 0; x < SALLE_WIDTH; x++){
            if(y == SALLE_HEIGHT - 1){
                // mur bas/vide
                salle->tiles[y][x] = 16;
            } else if(x == 0){
                // mur gauche
                salle->tiles[y][x] = 17; 
            } else if(x == SALLE_WIDTH - 1){
                // mur droite
                salle->tiles[y][x] = 24; 
            } else if(y == 0){
                // mur haut
                salle->tiles[y][x] = 21; 
            } else if(y == SALLE_HEIGHT - 2){
                // mur bas
                salle->tiles[y][x] = 14 + rand() % 2;
            } else if(y == 1){
                // sol relié au mur du haut
                salle->tiles[y][x] = 5 + rand() % 6;
                salle->tiles[y][x] == 8? salle->tiles[y-1][x] = 22 : 0; 
                salle->tiles[y][x] == 9? salle->tiles[y-1][x] = 23 : 0; 
            } else {
                // sol milieu
                salle->tiles[y][x] = rand() % 5;
            } 
        }
    }
    salle->tiles[SALLE_HEIGHT-2][0] = 19; 
    salle->tiles[SALLE_HEIGHT-2][SALLE_WIDTH-1] = 26;
}

void connecterSalles(Salle* salle1, Salle* salle2, int direction){
    int xDebut = (SALLE_WIDTH - 4) / 2 ;
    int yDebut = (SALLE_HEIGHT - 4) / 2 + 1;
    //en fct de la direction on creuse un passage pour les salles
    switch(direction){
        case DROITE:
            salle1->tiles[yDebut-2][SALLE_WIDTH - 1] = 21;
            salle1->tiles[yDebut-1][SALLE_WIDTH - 1] = 5 + rand() % 3;
            salle1->tiles[yDebut + 3][SALLE_WIDTH - 1] = 27;
            salle2->tiles[yDebut-2][0] = 21;
            salle2->tiles[yDebut-1][0] = 5 + rand() % 3;
            salle2->tiles[yDebut + 3][0] = 20;
            for(int i = 0; i < 3; i++) {
                salle1->tiles[yDebut + i][SALLE_WIDTH - 1] = 0;
                salle2->tiles[yDebut + i][0] = 0;
            }
            break;
        case BAS:
            salle1->tiles[SALLE_HEIGHT - 1][xDebut-1] = 17;
            salle1->tiles[SALLE_HEIGHT - 1][xDebut + 4] = 24;
            salle1->tiles[SALLE_HEIGHT - 2][xDebut + 4] = 27;
            salle1->tiles[SALLE_HEIGHT - 2][xDebut-1] = 20;
            for(int i = 0; i < 4; i++) {
                salle1->tiles[SALLE_HEIGHT - 1][xDebut + i] = 0; 
                salle1->tiles[SALLE_HEIGHT - 2][xDebut + i] = 0;
                salle2->tiles[0][xDebut + i] = 0;
                salle2->tiles[1][xDebut + i] = 0; 
            }
            break;
        case GAUCHE:
            salle1->tiles[yDebut-2][0] = 21;
            salle1->tiles[yDebut-1][0] = 5 + rand() % 3;
            salle1->tiles[yDebut + 3][0] = 20;
            salle2->tiles[yDebut-2][SALLE_WIDTH - 1] = 21;
            salle2->tiles[yDebut-1][SALLE_WIDTH - 1] = 5 + rand() % 3;
            salle2->tiles[yDebut + 3][SALLE_WIDTH - 1] = 27;
            for(int i = 0; i < 3; i++) {
                salle1->tiles[yDebut + i][0] = 0; 
                salle2->tiles[yDebut + i][SALLE_WIDTH - 1] = 0; 
            }
            break;
        case HAUT:
            salle2->tiles[SALLE_HEIGHT - 1][xDebut-1] = 17;
            salle2->tiles[SALLE_HEIGHT - 1][xDebut + 4] = 24;
            salle2->tiles[SALLE_HEIGHT - 2][xDebut-1] = 20;
            salle2->tiles[SALLE_HEIGHT - 2][xDebut + 4] = 27;
            for(int i = 0; i < 4; i++) {
                salle1->tiles[0][xDebut + i] = 0;
                salle1->tiles[1][xDebut + i] = 0;
                salle2->tiles[SALLE_HEIGHT - 1][xDebut + i] = 0;
                salle2->tiles[SALLE_HEIGHT - 2][xDebut + i] = 0;
            }
            break;
    }
}

//verifie si tile est un mur ou pas
int isWall(int tile){
    return (tile > ATLAS_MAP -1); //la premiere ligne de l atlas est faite de sol
}

void initMap(void) {
    initRandom(); 
    cleanupMap();

    //on construit un arbre de connexions
    int totalSalles = ETAGE_TAILLE * ETAGE_TAILLE; //nbr init de salles dans l'etage
    int sallesActives[totalSalles]; //tab de 1 et 0 en fct de si la salle est active ou pas
    int arbreParents[NB_SALLES_ACTIVES - 1]; //tab de stockage parent des connexions
    int arbreEnfants[NB_SALLES_ACTIVES - 1]; //tab de stockage enfant 
    int arbreDirections[NB_SALLES_ACTIVES - 1]; //tab de stockage direction des connexions
    nbrSallesActives = 0;
    int nbConnexions = 0;

    memset(sallesActives, 0, sizeof(sallesActives));

    for(int i = 0; i < totalSalles; i++){
        etage[i] = NULL;
    }

    int premiereSalle = rand() % totalSalles; //ou spawn
    sallesActives[premiereSalle] = 1;  //marque la salle comme active
    listeActives[nbrSallesActives++] = premiereSalle;

    while(nbrSallesActives < NB_SALLES_ACTIVES){
        int frontParents[ETAGE_TAILLE * ETAGE_TAILLE * 4]; 
        int frontEnfants[ETAGE_TAILLE * ETAGE_TAILLE * 4];
        int frontDirections[ETAGE_TAILLE * ETAGE_TAILLE * 4];
        int nbrFront = 0;

        for(int i = 0; i < nbrSallesActives; i++){
            int parent = listeActives[i];

            for(int direction = 0; direction < 4; direction++){
                int enfant = getIDSalleVoisine(parent, direction);

                if(enfant == -1 || sallesActives[enfant]){
                    continue;
                }

                frontParents[nbrFront] = parent;
                frontEnfants[nbrFront] = enfant;
                frontDirections[nbrFront] = direction;
                nbrFront++;
            }
        }

        if(nbrFront == 0){
            break;
        }

        int choix = rand() % nbrFront;
        int parent = frontParents[choix];
        int enfant = frontEnfants[choix];
        int direction = frontDirections[choix];

        sallesActives[enfant] = 1;
        listeActives[nbrSallesActives++] = enfant;
        arbreParents[nbConnexions] = parent;
        arbreEnfants[nbConnexions] = enfant;
        arbreDirections[nbConnexions] = direction;
        nbConnexions++;
    }

    for(int i = 0; i < totalSalles; i++){
        if(!sallesActives[i]){
            continue;
        }

        etage[i] = malloc(sizeof(Salle));
        etage[i]->mapID = i;
        etage[i]->xSalle = i % ETAGE_TAILLE;
        etage[i]->ySalle = i / ETAGE_TAILLE;
        genererSalle(etage[i]);
    }

    for(int i = 0; i < nbConnexions; i++){
        connecterSalles(etage[arbreParents[i]], etage[arbreEnfants[i]], arbreDirections[i]);
    }

    for(int y = 0; y < ETAGE_TAILLE; y++){
        for(int x = 0; x < ETAGE_TAILLE; x++){
            int idx = y * ETAGE_TAILLE + x;

            if(etage[idx] == NULL){
                continue;
            }

            if(x < ETAGE_TAILLE - 1 && etage[idx + 1] != NULL && rand() % 3 == 0){
                connecterSalles(etage[idx], etage[idx + 1], DROITE);
            }

            if(y < ETAGE_TAILLE - 1 && etage[idx + ETAGE_TAILLE] != NULL && rand() % 3 == 0){
                connecterSalles(etage[idx], etage[idx + ETAGE_TAILLE], BAS);
            }
        }
    }

    currentMap = etage[getIDSalleRandom()];
}

void drawMap(SDL_Renderer* renderer){
    if(!currentMap){
        return;
    }

    SDL_Rect src;
    SDL_Texture* atlas = getAtlasTexture();
    for(int y = 0; y < SALLE_HEIGHT; y++){
        for(int x = 0; x < SALLE_WIDTH; x++){
            int tileID = currentMap->tiles[y][x];
            src = getTileRect(tileID, ATLAS_MAP);

            SDL_Rect destRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderCopy(renderer, atlas, &src, &destRect);
        }
    }
}

void drawMob(SDL_Renderer* renderer, Mob* mob) {
    if(!mob){
        return;
    }

    SDL_Rect src = getTileRect(0, ATLAS_PERSO);
    SDL_Texture* mobAtlas = getMobAtlasTexture();
    SDL_Rect destRect = {mob->xTile * TILE_SIZE, mob->yTile * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, mobAtlas, &src, &destRect);
}

void cleanupMap(void) {
    for(int i = 0; i < ETAGE_TAILLE * ETAGE_TAILLE; i++) {
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

    switch(direction) {
        case DROITE:
            newX = currentX + 1; //on va a la salle de droite
            *playerX = 0; 
            break;
        case BAS: 
            newY = currentY + 1;
            *playerY = 0;
            break;
        case GAUCHE:
            newX = currentX - 1;
            *playerX = SALLE_WIDTH - 1;
            break;
        case HAUT:
            newY = currentY - 1;
            *playerY = SALLE_HEIGHT - 1;
            break;
    }
    
    //verification des limites
    if(newX < 0 || newX >= ETAGE_TAILLE || newY < 0 || newY >= ETAGE_TAILLE) {
        return; //pas de salle en dehors de la grille
    }
    
    //on change la salle actuelle
    idNouvelleSalle = newY * ETAGE_TAILLE + newX;
    if(etage[idNouvelleSalle] == NULL){
        return;
    }

    currentMap = etage[idNouvelleSalle];
}

void changeRoom(int direction, int* playerX, int* playerY) {
    changeSalle(direction, playerX, playerY);
}
