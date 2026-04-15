#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../lib/map.h"
#include "../lib/atlas.h"
#include "../lib/def.h"

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

static int estSalleCompacte(const Salle* salle){
    return salle && (salle->type == SALLE_COFFRE || salle->type == SALLE_TROC);
}

static int directionOpposee(int direction){
    switch(direction){
        case DROITE: return GAUCHE;
        case GAUCHE: return DROITE;
        case BAS: return HAUT;
        case HAUT: return BAS;
        default: return direction;
    }
}

static void prolongerCouloirSalleCompacte(Salle* salle, int direction, int xDebut, int yDebut){
    if(!estSalleCompacte(salle)){
        return;
    }

    switch(direction){
        case DROITE:
            for(int x = SALLE_WIDTH - 6; x < SALLE_WIDTH; x++){
                    salle->tiles[yDebut -1][x] = 5 + rand() % 3;
                    salle->tiles[yDebut -2][x] = 21;
                    salle->tiles[yDebut +3][x] = 14 + rand() % 2;
            }
            for(int y = yDebut; y < yDebut + 3; y++){
                for(int x = SALLE_WIDTH - 6; x < SALLE_WIDTH; x++){
                    salle->tiles[y][x] = rand() % 2;
                }
            }
            salle->tiles[yDebut + 3][SALLE_WIDTH - 6] = 27;
            break;
        case GAUCHE:
            for(int x = 0; x <= 5; x++){
                    salle->tiles[yDebut -1][x] = 5 + rand() % 3;
                    salle->tiles[yDebut -2][x] = 21;
                    salle->tiles[yDebut +3][x] = 14 + rand() % 2;
        }
            for(int y = yDebut; y < yDebut + 3; y++){
                for(int x = 0; x <= 5; x++){
                    salle->tiles[y][x] = rand() % 2;
                }
            }
            salle->tiles[yDebut + 3][5] = 20;
            break;
        case BAS:
            for(int y = SALLE_HEIGHT - 4; y < SALLE_HEIGHT; y++){
                    salle->tiles[y][xDebut -1] = 17;
                    salle->tiles[y][xDebut +4] = 24;
            }
            for(int y = SALLE_HEIGHT - 4; y < SALLE_HEIGHT; y++){
                for(int x = xDebut; x < xDebut + 4; x++){
                    salle->tiles[y][x] = rand() % 2;
                }
            }
            salle->tiles[SALLE_HEIGHT - 4][xDebut - 1] = 20;
            salle->tiles[SALLE_HEIGHT - 4][xDebut + 4] = 27;
            break;
        case HAUT:
            for(int y = 0; y <= 2; y++){
                    salle->tiles[y][xDebut -1] = 17;
                    salle->tiles[y][xDebut +4] = 24;
            }
            for(int y = 0; y <= 3; y++){
                for(int x = xDebut; x < xDebut + 4; x++){
                    salle->tiles[y][x] = rand() % 2;
                }
            }
            salle->tiles[2][xDebut - 1] = 21;
            salle->tiles[2][xDebut + 4] = 21;
            break;
    }
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

//le spawn
static void genererSalleSpawn(Salle* salle){
    salle->type = SALLE_SPAWN;
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
                salle->tiles[y][x] = 5 + rand() % 3;
            } else {
                // sol milieu
                salle->tiles[y][x] = rand() % 2;
            } 
        }
    }
    salle->tiles[SALLE_HEIGHT-2][0] = 19; 
    salle->tiles[SALLE_HEIGHT-2][SALLE_WIDTH-1] = 26;
}

//une salle normale (tiles de combat)
static void genererSalleNormale(Salle* salle){
    salle->type = SALLE_NORMALE;
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
    genererSalleSpawn(salle);
    salle->type = SALLE_BOSS;
}

static void genererSalleTroc(Salle* salle){
    salle->type = SALLE_TROC;
    //tout en vide d'abord
    for(int y = 0; y < SALLE_HEIGHT; y++){
        for(int x = 0; x < SALLE_WIDTH; x++){
            salle->tiles[y][x] = 16;
        }
    }

    //murs internes de la petite pièce
    for(int y = 2; y <= SALLE_HEIGHT - 4; y++){
        salle->tiles[y][5] = 17;
        salle->tiles[y][SALLE_WIDTH - 6] = 24;
    }
    for(int x = 6; x <= SALLE_WIDTH - 7; x++){
        salle->tiles[2][x] = 21;
        salle->tiles[3][x] = 5 + rand() % 3;
        salle->tiles[SALLE_HEIGHT - 4][x] = 14 + rand() % 2;
    }
    salle->tiles[SALLE_HEIGHT - 4][5] = 19;
    salle->tiles[SALLE_HEIGHT - 4][SALLE_WIDTH - 6] = 26;

    //sol interieur
    for(int y = 4; y <= SALLE_HEIGHT - 5; y++){
        for(int x = 6; x <= SALLE_WIDTH - 7; x++){
            salle->tiles[y][x] = rand() % 2;
        }
    }
    //le marchand est un mob special gere hors generation de tiles
}

static void genererSalleCoffre(Salle* salle){
    salle->type = SALLE_COFFRE;
    genererSalleTroc(salle);
    //tuile coffre au centre de la salle
    salle->tiles[SALLE_HEIGHT / 2][SALLE_WIDTH / 2] = 11;
}

//genere une salle en fonction de son type
void genererSalle(Salle* salle, SalleType type){
    if(!salle) return;
    
    switch(type){
        case SALLE_SPAWN:
            genererSalleSpawn(salle);
            break;
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

static int getIDSalleRandomExcluantListe(const int *idsExclus, int nbExclus){
    if(nbrSallesActives == 0){
        return 0;
    }

    for(int tentative = 0; tentative < 64; tentative++){
        int id = listeActives[rand() % nbrSallesActives];
        int exclu = 0;
        for(int i = 0; i < nbExclus; i++){
            if(id == idsExclus[i]){
                exclu = 1;
                break;
            }
        }
        if(!exclu){
            return id;
        }
    }

    for(int k = 0; k < nbrSallesActives; k++){
        int id = listeActives[k];
        int exclu = 0;
        for(int i = 0; i < nbExclus; i++){
            if(id == idsExclus[i]){
                exclu = 1;
                break;
            }
        }
        if(!exclu){
            return id;
        }
    }

    return listeActives[0];
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

    prolongerCouloirSalleCompacte(salle1, direction, xDebut, yDebut);
    prolongerCouloirSalleCompacte(salle2, directionOpposee(direction), xDebut, yDebut);
}

//verifie si tile est un mur ou pas
int isWall(int tile){
    return (tile > 10); //les tuiles de sol sont entre 0 et 10, les murs sont au dessus
}

//verifie si tile est un sol de combat ou pas
int isCombatTile(int tile){
    return (tile == 2 || tile == 8 || tile == 9); //sols de combat
}

//met en place la carte en fct de l'etage
void initMapParEtage(int etageActuel, int *bossRoomID, int *miniBossRoomID, int *trocRoomID){
    initRandom(); 
    cleanupMap();

    
    int totalSalles = ETAGE_TAILLE * ETAGE_TAILLE; //nbr max de salles dans l'etage
    int sallesActives[totalSalles]; //tab pour activer les salles qui seront dans l'etage
    //alogorithme avec arbre couvrant pour generer les salles :
    int arbreParents[NB_SALLES_ACTIVES - 1]; 
    int arbreEnfants[NB_SALLES_ACTIVES - 1];
    int arbreDirections[NB_SALLES_ACTIVES - 1];
    SalleType typeParSalle[ETAGE_TAILLE * ETAGE_TAILLE]; //ici on stock les types de salles
    nbrSallesActives = 0;
    int nbConnexions = 0;

    memset(sallesActives, 0, sizeof(sallesActives));

    //initialisation des tab
    for(int i = 0; i < totalSalles; i++){
        etage[i] = NULL;
        typeParSalle[i] = SALLE_NORMALE;
    }

    int premiereSalle = rand() % totalSalles;
    sallesActives[premiereSalle] = 1;
    listeActives[nbrSallesActives++] = premiereSalle;

    while(nbrSallesActives < NB_SALLES_ACTIVES){
        int frontParents[ETAGE_TAILLE * ETAGE_TAILLE * 4];
        int frontEnfants[ETAGE_TAILLE * ETAGE_TAILLE * 4];
        int frontDirections[ETAGE_TAILLE * ETAGE_TAILLE * 4];
        int nbrFront = 0;

        //pour chaque salle active, on check les salles voisines et on les ajoute aux tab de front si pas actives
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

        //on arrete si plus de salles voisines dispo
        if(nbrFront == 0)break;
        //ici en random on choisiit une salle du front + l'active et la connecte a son 'parent'
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
    //mtn on assigne les salles a des types
    int idSalleSpawn = getIDSalleRandom();
    int idSalleSpeciale = getIDSalleRandomExcluant(idSalleSpawn);
    int idSalleBoss = -1;
    int idSalleMiniBoss = -1;
    typeParSalle[idSalleSpawn] = SALLE_SPAWN;
    SalleType typeSpeciale = (rand() % 2 == 0) ? SALLE_COFFRE : SALLE_TROC;
    typeParSalle[idSalleSpeciale] = typeSpeciale;
    if(trocRoomID){
        *trocRoomID = (typeSpeciale == SALLE_TROC) ? idSalleSpeciale : -1;
    }
    if(etageActuel >= 3){
        int exclusBoss[2] = {idSalleSpawn, idSalleSpeciale};
        idSalleBoss = getIDSalleRandomExcluantListe(exclusBoss, 2);
        typeParSalle[idSalleBoss] = SALLE_BOSS;
        int exclusMiniBoss[3] = {idSalleSpawn, idSalleSpeciale, idSalleBoss};
        idSalleMiniBoss = getIDSalleRandomExcluantListe(exclusMiniBoss, 3);
    }else{
        int exclusMiniBoss[2] = {idSalleSpawn, idSalleSpeciale};
        idSalleMiniBoss = getIDSalleRandomExcluantListe(exclusMiniBoss, 2);
        idSalleBoss = idSalleMiniBoss;
    }

    for(int i = 0; i < totalSalles; i++){
        if(!sallesActives[i])continue;

        etage[i] = malloc(sizeof(Salle));
        etage[i]->mapID = i;
        etage[i]->xSalle = i % ETAGE_TAILLE;
        etage[i]->ySalle = i / ETAGE_TAILLE;
        genererSalle(etage[i], typeParSalle[i]);
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

    currentMap = etage[idSalleSpawn];
    if(bossRoomID){
        *bossRoomID = idSalleBoss;
    }
    if(miniBossRoomID){
        *miniBossRoomID = (etageActuel >= 3) ? idSalleMiniBoss : -1;
    }
}

void drawMap(SDL_Renderer* renderer){
    if(!currentMap){
        return;
    }
    SDL_Rect src;
    SDL_Texture* atlas = getAtlasMap();
    for(int y = 0; y < SALLE_HEIGHT; y++){
        for(int x = 0; x < SALLE_WIDTH; x++){
            int tileID = currentMap->tiles[y][x];
            src = getTileRect(tileID, ATLAS_MAP);

            SDL_Rect destRect = {x * TAILLE_TUILE, y * TAILLE_TUILE, TAILLE_TUILE, TAILLE_TUILE};
            SDL_RenderCopy(renderer, atlas, &src, &destRect);
        }
    }
}

void drawMob(SDL_Renderer* renderer, Mob* mob) {
    if(!mob)return; //mesure de securite
    SDL_Texture* mobAtlas = getAtlasMob();

    SDL_Rect src;
    SDL_Rect destRect;

    //affichage du mob en 128*128
    if(mob->spriteID == 2){
        src = (SDL_Rect){128, 0, 128, 128};
        destRect = (SDL_Rect){mob->xTile * TAILLE_TUILE, mob->yTile * TAILLE_TUILE, 128, 128};
    }
    else{
        src = getTileRect(mob->spriteID, ATLAS_PERSO);
        destRect = (SDL_Rect){mob->xTile * TAILLE_TUILE, mob->yTile * TAILLE_TUILE, TAILLE_TUILE, TAILLE_TUILE};
    }

    SDL_RenderCopy(renderer, mobAtlas, &src, &destRect);
}

void drawPlayer(SDL_Renderer* renderer, Player * player){
    SDL_Rect src;
    SDL_Rect dest;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if(!player || !player->texture)return; //mesure de securite

    src = getTileRect(player->spriteID, ATLAS_PERSO);
    if(player->facing == GAUCHE){
        flip = SDL_FLIP_HORIZONTAL;}
    dest.x = player->xTile * TAILLE_TUILE;
    dest.y = player->yTile * TAILLE_TUILE;
    dest.w = TAILLE_TUILE;
    dest.h = TAILLE_TUILE;
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
    if(etage[idNouvelleSalle] == NULL)return; //si pas de salle,stop
    currentMap = etage[idNouvelleSalle];
}

void setCurrentMapByID(int mapID){
    if(mapID < 0 || mapID >= ETAGE_TAILLE * ETAGE_TAILLE)return;//verification limites
    if(etage[mapID] == NULL)return; //si existe pas,stop
    currentMap = etage[mapID];
}

//genere une salle de boss a partir de son ID 
void genererSalleBossByID(int mapID){
    regenererSalleByID(mapID, SALLE_BOSS); 
}