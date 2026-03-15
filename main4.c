#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "structs.h"
#include "system/map.h"
#include "system/atlas.h"
#include "system/utilitaire.h"
#include "system/inventaire.h"
#include "system/text.h"
#include "system/combat/combat.h"
#include "system/combat/combat_aff.h"
#include "system/combat/combat_attaque.h"
#include "def.h"

//commande de compilation 
//gcc -o main4 main4.c ./system/atlas.c ./system/map.c ./system/inventaire.c ./system/utilitaire.c ./system/text.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

Game game;
Player player;
Mob mobTest = {.mapID=1, .xTile=5, .yTile=5};
Bouton btnF = {.couleurFond.r = 80, .couleurFond.g = 80, .couleurFond.b = 80, .couleurFond.a = 255, .couleurTexte.r = 255, .couleurTexte.g = 255, .couleurTexte.b = 255, .couleurTexte.a = 255, .texte = "F"};


//evite collision avec les mobs
/*int checkMobCollision(int x, int y) {
    if (currentMap->mapID == mobTest.mapID) {
        if (mobTest.xTile == x && mobTest.yTile == y) {
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

    if(TTF_Init() < 0){
        fprintf(stderr, "Erreur TTF_Init : %s", TTF_GetError());
        return -1;
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    
    initAtlas(game.renderer);
    initAtlasItem(game.renderer);
    initAtlasMenu(game.renderer);
    initMobAtlas(game.renderer);
    initAtlasPerso(game.renderer);
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
    cleanupAtlasItem();
    cleanupAtlasMenu();
    cleanupMobAtlas();
    cleanupAtlasPerso();
    cleanupMap();
    if(game.renderer != NULL)
        SDL_DestroyRenderer(game.renderer);
    if(game.window != NULL)
        SDL_DestroyWindow(game.window);
    SDL_Quit();
}

int main(int argc, char *argv[]){
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

    initMap();
//----------------------------------------création des items -----------------------------------
    int pause = 0;/** sert à indiquer si le menu est ouvert 1 ou fermé 0 */
    int sortie = 0;/* variable servant à arrêter le programme */
    int menu = 0;/** variable servant à indiquer si un menu est ouvert */
    int itemObtenu[10];
    Fighter player1 = {40, 140, 120, 100};
    item_t potion;
    potion.f = soin1PV;
    item_t potion2;
    potion2.f = soin5PV;
    item_t key;
    key.f = NULL;
    item_t * listeItem[3];
    listeItem[0] = &potion;
    listeItem[1] = &potion2;
    listeItem[2] = &key;
//----------------------chargement des variables de menu ---------------------------
SDL_Rect destEchap = {1180,50,TAILLE_SPRITE/2,TAILLE_SPRITE/2};//position du bouton echap
//----------------------Chargement d'image ------------------------------------------
    SDL_Surface * surface = SDL_GetWindowSurface(game.window);
    SDL_Texture * Chiffre = NULL;
    if (chargerImage("Img/chiffreTest.bmp", game.renderer, &Chiffre) != 0) {
        fprintf(stderr, "Erreur chargement Chiffre\n");
        cleanup();
        return 1;
    }
//---------------------------------------------menu de départ
    SDL_Rect play = {SCREEN_WIDTH/2-TAILLE_MENU,SCREEN_HEIGHT/2.5-TAILLE_MENU,TAILLE_MENU,TAILLE_MENU};
    SDL_Rect continu = {SCREEN_WIDTH/2-TAILLE_MENU,SCREEN_HEIGHT/2-TAILLE_MENU,TAILLE_MENU,TAILLE_MENU};
    SDL_Rect quit = {SCREEN_WIDTH/2-TAILLE_MENU,SCREEN_HEIGHT/1.5-TAILLE_MENU,TAILLE_MENU,TAILLE_MENU};
    SDL_Rect playF;
    SDL_Rect quitF;
    SDL_Rect continuF;
    int sourisX,sourisY;
    while(sortie==0){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(detecterButtonClique(&event,&play)){
                    sortie = 2;
                }
                else if (detecterButtonClique(&event,&quit)){
                    SDL_DestroyTexture(Chiffre);
                    cleanup();
                    return 0;
                }
                else if(detecterButtonClique(&event,&continu)){
                    sortie =1;
                }
            }
        }
        SDL_GetMouseState(&(sourisX),&(sourisY));
        if((play.x<sourisX)&&(play.x+TAILLE_MENU>sourisX)&&(play.y<sourisY)&&(play.y+TAILLE_MENU>sourisY)){
            playF = getTileRect(1,ATLAS_BOUTON);
        }
        else{
            playF = getTileRect(0,ATLAS_BOUTON);     
        }
        if((quit.x<sourisX)&&(quit.x+TAILLE_MENU>sourisX)&&(quit.y<sourisY)&&(quit.y+TAILLE_MENU>sourisY)){
            quitF = getTileRect(3,ATLAS_BOUTON);
        }
        else{
            quitF = getTileRect(2,ATLAS_BOUTON);
        }
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255); 
        SDL_RenderClear(game.renderer);
        if(fichierExiste(FICHIER_DATA)){
            if((continu.x<sourisX)&&(continu.x+TAILLE_MENU>sourisX)&&(continu.y<sourisY)&&(continu.y+TAILLE_MENU>sourisY)){
                continuF = getTileRect(7,ATLAS_BOUTON);
            }
            else{
                continuF = getTileRect(6,ATLAS_BOUTON);
            }
            SDL_RenderCopy(game.renderer,getAtlasMenu(),&continuF,&continu);
        }
        SDL_RenderCopy(game.renderer,getAtlasMenu(),&playF,&play);
        SDL_RenderCopy(game.renderer,getAtlasMenu(),&quitF,&quit);
        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }
//-------------------lecture des données -----------------------------------------
    FILE * f;
    if(sortie==2){//si le fichier data.txt on le créé
        f = fopen(FICHIER_DATA,"w");
        SDL_Rect DestGla = {200,100,TAILLE_SPRITE,TAILLE_SPRITE};
        SDL_Rect glaF = getTileRect(1,ATLAS_PERSO);
        SDL_Rect archerF = getTileRect(0,ATLAS_PERSO);
        SDL_Rect DestArc = {400,100,TAILLE_SPRITE,TAILLE_SPRITE};
        SDL_Rect DestLan = {600,100,TAILLE_SPRITE,TAILLE_SPRITE};
        SDL_Rect lanF = getTileRect(2,ATLAS_PERSO);
        SDL_RenderCopy(game.renderer,getAtlasPerso(),&glaF,&DestGla);
        SDL_RenderCopy(game.renderer,getAtlasPerso(),&archerF,&DestArc);
        SDL_RenderCopy(game.renderer,getAtlasPerso(),&lanF,&DestLan);
        SDL_RenderPresent(game.renderer);
        int choixPerso = 0;
        while(choixPerso==0){//choix de la classe 
            SDL_Event event;
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_MOUSEBUTTONDOWN){
                    if(detecterButtonClique(&event,&DestGla)){
                        fprintf(f,"classeID=%d\n",GLADIATEUR);
                        fprintf(f,"pv_max=%d\n",GLADIATEUR_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",GLADIATEUR_ATTACK);
                        fprintf(f,"stat_speed=%d\n",GLADIATEUR_SPEED);
                        choixPerso++;
                    }
                    else if(detecterButtonClique(&event,&DestArc)){
                        fprintf(f,"classeID=%d\n",ARCHER);
                        fprintf(f,"pv_max=%d\n",ARCHER_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",ARCHER_ATTACK);
                        fprintf(f,"stat_speed=%d\n",ARCHER_SPEED);
                        choixPerso++;
                    }
                    else if(detecterButtonClique(&event,&DestLan)){
                        fprintf(f,"classeID=%d\n",LANCIER);
                        fprintf(f,"pv_max=%d\n",LANCIER_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",LANCIER_ATTACK);
                        fprintf(f,"stat_speed=%d\n",LANCIER_SPEED);
                        choixPerso++;
                    }
                }
            }
        }
        fprintf(f,"nb_potions=%d\n",0);
        fprintf(f,"nb_superpotions=%d\n",0);
        fprintf(f,"nb_clés=%d\n",0);
        fclose(f);
    }
    f = fopen(FICHIER_DATA,"r");//chargement des données dans les variables locals 
    fscanf(f,"classeID=%d\n",&(player1.classeID));
    fscanf(f,"pv_max=%d\n",&(player1.max_hp));
    fscanf(f,"stat_attaque=%d\n",&(player1.attack));
    fscanf(f,"stat_speed=%d\n",&(player1.speed));
    fscanf(f,"nb_potions=%d\n",&(listeItem[0]->nb));
    fscanf(f,"nb_superpotions=%d\n",&(listeItem[1]->nb));
    fscanf(f,"nb_clés=%d\n",&(listeItem[2]->nb));
    fclose(f);
    player1.hp = player1.max_hp;
    sortie = 0;
//----------------------------------------------------------------------------------------------
    //position de depart du player
    player.xTile = 9;
    player.yTile = 7;
    player.texture = loadTexture("assets/batman.png"); //chemin du sprite du player
    atexit(cleanup); //nettoyera la memoire à la fermeture du programme
    ////////////////////////
    
    while(sortie==0){
        prepareScene();
        SDL_Event e;
        
        //gestion des evenements
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                sortie = 1;
            }
            if(menu==0){
                if((e.type == SDL_KEYDOWN)&&(e.key.keysym.sym == SDLK_ESCAPE)){//touche echap = arrêt du programme 
                    SDL_DestroyTexture(Chiffre);
                    sortie = 1;
                }
                //si joueur a cote du mob
                if(currentMap->mapID == mobTest.mapID){
                    if ((abs(player.xTile - mobTest.xTile) + abs(player.yTile - mobTest.yTile)) == 1){
                        //si appuie sur F, lance combat
                        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){
                            lancerCombat(game.renderer);
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
                    if(newX >= SALLE_WIDTH){
                        changeRoom(DROITE, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        if(!isWall(tile) && !checkMobCollision(newX, newY)){
                            player.xTile = newX;
                            player.yTile = newY;
                        }
                    }else if(newX < 0){
                        changeRoom(GAUCHE, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        if(!isWall(tile) && !checkMobCollision(newX, newY)){
                            player.xTile = newX;
                            player.yTile = newY;
                        }
                    }else if(newY >= SALLE_HEIGHT){
                        changeRoom(BAS, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        if(!isWall(tile) && !checkMobCollision(newX, newY)){
                            player.xTile = newX;
                            player.yTile = newY;
                        }
                    }else if(newY < 0){
                        changeRoom(HAUT, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        if(!isWall(tile) && !checkMobCollision(newX, newY)){
                            player.xTile = newX;
                            player.yTile = newY;
                        }
                    }else{
                        int tile = currentMap->tiles[newY][newX];
                        if(!isWall(tile) && !checkMobCollision(newX, newY)){
                            player.xTile = newX;
                            player.yTile = newY;
                        }
                    }
                }
                if(e.type == SDL_KEYUP){
                    if(e.key.keysym.sym == SDLK_TAB){//si la touche est tab est préssé
                        if(pause==0){
                            pause = 1;
                        }
                        else{
                            pause = 0;
                        }
                    }
                    if(e.key.keysym.sym == SDLK_a){//SERT a tester les drops de mob
                        //drawMap(game.renderer, mapOffsetX, mapOffsetY);
                        drawTexture(player.texture, player.xTile * TILE_SIZE, player.yTile * TILE_SIZE, 64, 64);
                        menu = dropItem(game.renderer,listeItem,itemObtenu,1,1,1,1,1,1);
                    }
                }
                if((e.type == SDL_MOUSEBUTTONDOWN)&&(pause)){
                    detecterItemUtilise(&e,listeItem,&player1);
                }
            }
            else{
                if(e.type == SDL_MOUSEBUTTONDOWN){
                    if(detecterButtonClique(&e,&destEchap)){
                        menu = 0;
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
        if(pause){
            afficherInventaire(game.renderer,Chiffre,listeItem,&player1);
        }
        if(menu){
            afficherItemObtenu(game.renderer,menu,itemObtenu);
        }
        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }
    SDL_DestroyTexture(Chiffre);
    cleanup();
    return 0;
}