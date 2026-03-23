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
    \date 14 février 2026
*/


Salle* currentMap = NULL;
Salle* etage[ETAGE_TAILLE * ETAGE_TAILLE];

//liste des salles reellement presentes dans l'etage
static int listeActives[NB_SALLES_ACTIVES]; 
static int nbrSallesActives = 0;

static void connecterSalles(Salle* salle1, Salle* salle2, int direction);

static void reinitialiserEventSalle(Salle* salle){
    salle->itemAtlasID = -1;
    salle->itemX = -1;
    salle->itemY = -1;
}

static void placerEventSalle(Salle* salle, int itemAtlasID){
    int minX = 1;
    int maxX = SALLE_WIDTH - 2;
    int minY = 2;
    int maxY = SALLE_HEIGHT - 3;

    salle->itemAtlasID = itemAtlasID;
    salle->itemX = minX + rand() % (maxX - minX + 1);
    salle->itemY = minY + rand() % (maxY - minY + 1);
}

int getIDSalleRandom(void){ //pour positionner le mini boss dans une salle random et mettre le spawn
    if(nbrSallesActives == 0) return 0;
    return listeActives[rand() % nbrSallesActives];
}

//retourne un ID de salle active different de celui passé en paramètre, ou celui passé si aucune autre salle disponible
int getIDSalleRandomExcluant(int mapIDExclu){
    if(nbrSallesActives == 0)return 0;
    if(nbrSallesActives == 1)return listeActives[0];

    for(int tentative = 0; tentative < 32; tentative++){
        int id = listeActives[rand() % nbrSallesActives];
        if(id != mapIDExclu){
            return id;
        }
    }

    for(int i = 0; i < nbrSallesActives; i++){
        if(listeActives[i] != mapIDExclu){
            return listeActives[i];
        }
    }
    return mapIDExclu;
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

//une salle normale (tiles de combat)
static void genererSalleNormale(Salle* salle){
    salle->type = SALLE_NORMALE;
    reinitialiserEventSalle(salle);
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

//salle de boss (sol simple avec le boss au milieu)
static void genererSalleBoss(Salle* salle){
    salle->type = SALLE_BOSS;
    reinitialiserEventSalle(salle);
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
                salle->tiles[y][x] = 14;
            } else if(y == 1){
                // sol relié au mur du haut
                salle->tiles[y][x] = 5;
            } else {
                // sol milieu
                salle->tiles[y][x] = 0;
            } 
        }
    }
    salle->tiles[SALLE_HEIGHT-2][0] = 19; 
    salle->tiles[SALLE_HEIGHT-2][SALLE_WIDTH-1] = 26;
}

static void genererSalleCoffre(Salle* salle){
    genererSalleBoss(salle);
    salle->type = SALLE_COFFRE;
    placerEventSalle(salle, 2);
}

static void genererSalleTroc(Salle* salle){
    genererSalleBoss(salle);
    salle->type = SALLE_TROC;
    placerEventSalle(salle, 1);
}

//genere une salle en fonction de son type
void genererSalle(Salle* salle, SalleType type){
    if(!salle) return;
    
    switch(type){
        case SALLE_NORMALE:
            genererSalleNormale(salle);
            break;
        case SALLE_COFFRE:
            genererSalleCoffre(salle);
            break;
        case SALLE_TROC:
            genererSalleTroc(salle);
            break;  
        case SALLE_BOSS:
            genererSalleBoss(salle);
            break;
        default:
            genererSalleNormale(salle);
            break;
    }
}

static void regenererSalleByID(int mapID, SalleType type){
    if(mapID < 0 || mapID >= ETAGE_TAILLE * ETAGE_TAILLE){
        return;
    }
    if(etage[mapID] == NULL){
        return;
    }

    genererSalle(etage[mapID], type);

    Salle *salle = etage[mapID];
    int voisinID;

    voisinID = getIDSalleVoisine(mapID, DROITE);
    if(voisinID != -1 && etage[voisinID] != NULL){
        connecterSalles(salle, etage[voisinID], DROITE);
    }

    voisinID = getIDSalleVoisine(mapID, BAS);
    if(voisinID != -1 && etage[voisinID] != NULL){
        connecterSalles(salle, etage[voisinID], BAS);
    }

    voisinID = getIDSalleVoisine(mapID, GAUCHE);
    if(voisinID != -1 && etage[voisinID] != NULL){
        connecterSalles(salle, etage[voisinID], GAUCHE);
    }

    voisinID = getIDSalleVoisine(mapID, HAUT);
    if(voisinID != -1 && etage[voisinID] != NULL){
        connecterSalles(salle, etage[voisinID], HAUT);
    }
}

static int getIDSalleRandomExcluantDeux(int mapIDExclu1, int mapIDExclu2){
    if(nbrSallesActives == 0){
        return 0;
    }

    for(int tentative = 0; tentative < 32; tentative++){
        int id = listeActives[rand() % nbrSallesActives];
        if(id != mapIDExclu1 && id != mapIDExclu2){
            return id;
        }
    }

    for(int i = 0; i < nbrSallesActives; i++){
        int id = listeActives[i];
        if(id != mapIDExclu1 && id != mapIDExclu2){
            return id;
        }
    }

    return getIDSalleRandomExcluant(mapIDExclu1);
}

static void connecterSalles(Salle* salle1, Salle* salle2, int direction){
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

//verifie si tile est un sol de combat ou pas
int isCombatTile(int tile){
    return (tile == 2 || tile == 8 || tile == 9); //sols de combat
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
        genererSalle(etage[i], SALLE_NORMALE);
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

void initMapParEtage(int etageActuel, int *bossRoomID, int *miniBossRoomID){
    initMap();

    int miniBossID = getIDSalleRandom(); //choisit salle pour miniBoss
    int salleSpecialeID = getIDSalleRandomExcluant(miniBossID);
    int bossID = getIDSalleRandomExcluantDeux(miniBossID, salleSpecialeID); //et Boss (forcement dans une salle diff)

    regenererSalleByID(salleSpecialeID, (rand() % 2 == 0) ? SALLE_COFFRE : SALLE_TROC);

    if(etageActuel >= 3){
        genererSalleBossByID(miniBossID);

        int spawnID = getIDSalleRandomExcluantDeux(miniBossID, salleSpecialeID);
        setCurrentMapByID(spawnID);

        bossID = getIDSalleRandomExcluantDeux(miniBossID, salleSpecialeID);
        if(nbrSallesActives > 3 && (bossID == spawnID || bossID == salleSpecialeID)){
            for(int tentative = 0; tentative < 16; tentative++){
                bossID = getIDSalleRandomExcluantDeux(miniBossID, salleSpecialeID);
                if(bossID != spawnID && bossID != salleSpecialeID){
                    break;
                }
            }
        }
    }

    if(bossRoomID){
        *bossRoomID = miniBossID;
    }
    if(miniBossRoomID){
        *miniBossRoomID = bossID;
    }
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

    if(currentMap->itemAtlasID >= 0){
        SDL_Rect srcItem = getTileRect(currentMap->itemAtlasID, ATLAS_ITEM);
        SDL_Rect destItem = {
            currentMap->itemX * TILE_SIZE,
            currentMap->itemY * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE
        };
        SDL_RenderCopy(renderer, getAtlasItem(), &srcItem, &destItem);
    }
}

void drawMob(SDL_Renderer* renderer, Mob* mob) {
    if(!mob){
        return;
    }

    SDL_Rect src = getTileRect(mob->spriteID, ATLAS_PERSO);
    SDL_Texture* mobAtlas = getMobAtlasTexture();
    SDL_Rect destRect = {mob->xTile * TILE_SIZE, mob->yTile * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, mobAtlas, &src, &destRect);
}

void drawPlayer(SDL_Renderer* renderer, Player * player){
    SDL_Rect src;
    SDL_Rect dest;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if(!player || !player->texture){ 
        return;
    }
    src = getTileRect(player->spriteID, ATLAS_PERSO);
    if(player->facing == GAUCHE){
        flip = SDL_FLIP_HORIZONTAL;
    }
    dest.x = player->xTile * TILE_SIZE;
    dest.y = player->yTile * TILE_SIZE;
    dest.w = TILE_SIZE;
    dest.h = TILE_SIZE;
    SDL_RenderCopyEx(renderer, player->texture, &src, &dest, 0.0, NULL, flip);
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

void setCurrentMapByID(int mapID){
    if(mapID < 0 || mapID >= ETAGE_TAILLE * ETAGE_TAILLE){
        return;
    }
    if(etage[mapID] == NULL){
        return;
    }

    currentMap = etage[mapID];
}

//genere une salle de boss a partir de son ID 
void genererSalleBossByID(int mapID){
    regenererSalleByID(mapID, SALLE_BOSS); 
}