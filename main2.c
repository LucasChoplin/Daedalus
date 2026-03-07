#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "system/inventaire.h"
#include "system/map.h"
#include "system/atlas.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define GLADIATEUR_MAX_HP 200
#define GLADIATEUR_ATTACK 120
#define GLADIATEUR_SPEED 100
#define ARCHER_MAX_HP 100
#define ARCHER_ATTACK 220
#define ARCHER_SPEED 100
#define LANCIER_MAX_HP 100
#define LANCIER_ATTACK 120
#define LANCIER_SPEED 200

Game game;
Player player;

//commande de compilation ::
// gcc -o main main2.c ./system/atlas.c ./system/map.c ./system/inventaire.c ./system/utilitaire.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

//initialisation de SDL et creation de la fenetre et du renderer
int initSDL(){
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

    int mapPixelWidth = MAP_WIDTH * TILE_SIZE;
    int mapPixelHeight = MAP_HEIGHT * TILE_SIZE;
    int mapOffsetX = (SCREEN_WIDTH - mapPixelWidth) / 2;
    int mapOffsetY = (SCREEN_HEIGHT - mapPixelHeight) / 2;

    //position de depart du player
    player.xTile = 2;
    player.yTile = 2;
    player.texture = loadTexture("assets/batman.png"); //chemin du sprite du player
    atexit(cleanup); //nettoyera la memoire à la fermeture du programme
    ////////////////////////
//-------------------------- création des items --------------------
    int pause = 0;/** sert à indiquer si le menu est ouvert 1 ou fermé 0 */
    int sortie = 0;/* variable servant à arrêter le programme */
    int itemDraw = 0;/* variable servant à indiquer si il y'a un item à afficher*/
    int timeDraw = 0;/* variable servant à conserver le nombre de fois qu'un item à été dessiner*/
    initAtlasItem(game.renderer);
    SDL_Texture * itemAfficher;
    Fighter player1 = {40, 140, 120, 100};
    item_t potion;
    potion.nb = 10;
    potion.f = soin1PV;
    //chargerImage("Img/potion.bmp",game.renderer,&potion.t);
    item_t potion2;
    potion2.nb = 5;
    potion2.f = soin5PV;
    //chargerImage("Img/potion2.bmp",game.renderer,&potion2.t);
    item_t * listeItem[3];
    listeItem[0] = &potion;
    listeItem[1] = &potion2;
//----------------------Chargement d'image ------------------------------------------
    SDL_Surface * surface = SDL_GetWindowSurface(game.window);
    SDL_Texture * Chiffre;
    SDL_Texture * atlasItem;
    chargerImage("Img/chiffreTest.bmp",game.renderer,&Chiffre);
    SDL_Rect srcDigits[10]; 
    int digitWidth = 32; 
    int digitHeight = 32; // hauteur d’un chiffre 
    for (int i = 0; i < 10; i++) { 
        srcDigits[i].x = i * digitWidth; 
        srcDigits[i].y = 0; 
        srcDigits[i].w = digitWidth; 
        srcDigits[i].h = digitHeight; 
    }
//-------------------lecture des données -----------------------------------------
    FILE * f;
    if(!FichierExiste(FICHIER_DATA)){//si le fichier data.txt on le créé
        f = fopen(FICHIER_DATA,"w");
        SDL_Texture * Gladiateur;
        SDL_Rect DestGla = {200,100,TAILLE_SPRITE,TAILLE_SPRITE};
        chargerImage("Img/gladiateur.bmp",game.renderer,&Gladiateur);
        SDL_Texture * Archer;
        SDL_Rect DestArc = {400,100,TAILLE_SPRITE,TAILLE_SPRITE};
        chargerImage("Img/archer.bmp",game.renderer,&Archer);
        SDL_Texture * Lancier;
        SDL_Rect DestLan = {600,100,TAILLE_SPRITE,TAILLE_SPRITE};
        chargerImage("Img/lancier.bmp",game.renderer,&Lancier);
        SDL_RenderCopy(game.renderer,Gladiateur,NULL,&DestGla);
        SDL_RenderCopy(game.renderer,Archer,NULL,&DestArc);
        SDL_RenderCopy(game.renderer,Lancier,NULL,&DestLan);
        SDL_RenderPresent(game.renderer);
        int choixPerso = 0;
        while(choixPerso==0){//choix de la classe 
            SDL_Event event;
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_MOUSEBUTTONDOWN){
                    if(detecterButtonClique(&event,&DestGla)){
                        fprintf(f,"pv_max=%d\n",GLADIATEUR_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",GLADIATEUR_ATTACK);
                        fprintf(f,"stat_speed=%d\n",GLADIATEUR_SPEED);
                        choixPerso++;
                    }
                    else if(detecterButtonClique(&event,&DestArc)){
                        fprintf(f,"pv_max=%d\n",ARCHER_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",ARCHER_ATTACK);
                        fprintf(f,"stat_speed=%d\n",ARCHER_SPEED);
                        choixPerso++;
                    }
                    else if(detecterButtonClique(&event,&DestLan)){
                        fprintf(f,"pv_max=%d\n",LANCIER_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",LANCIER_ATTACK);
                        fprintf(f,"stat_speed=%d\n",LANCIER_SPEED);
                        choixPerso++;
                    }
                }
            }
        }
        fprintf(f,"nb_potions=%d\n",0);
        fprintf(f,"nb_Superpotions=%d\n",0);
        fclose(f);
        SDL_DestroyTexture(Gladiateur);
        SDL_DestroyTexture(Archer);
        SDL_DestroyTexture(Lancier);
    }
    f = fopen(FICHIER_DATA,"r");//chargement des données dans les variables locals 
    fscanf(f,"pv_max=%d\n",&(player1.max_hp));
    fscanf(f,"stat_attaque=%d\n",&(player1.attack));
    fscanf(f,"stat_speed=%d\n",&(player1.speed));
    fscanf(f,"nb_potions=%d\n",&(listeItem[0]->nb));
    fscanf(f,"nb_Superpotions=%d\n",&(listeItem[1]->nb));
    fclose(f);
    player1.hp = player1.max_hp;
//--------------------------------------------------------------------------------
    while(sortie==0){
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
                    case SDLK_ESCAPE://touche echap = arrêt du programme 
                        sortie = 1;
                        break;
                }
                //on verifie les collisions
                int tile = map[newY][newX];
                if(!isWall(tile) && newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT){
                    player.xTile = newX;
                    player.yTile = newY;
                }
            }
            if(e.type == SDL_KEYUP){
                if(e.key.keysym.sym == SDLK_TAB){//si la touche est tab 
                    printf("Tab détecté");
                    if(pause==0){
                        pause = 1;
                    }
                    else{
                        pause = 0;
                    }
                }
                if(e.key.keysym.sym == SDLK_a){
                    itemDraw = dropItem(listeItem,0,&itemAfficher);
                }
            }
            if((e.type == SDL_MOUSEBUTTONDOWN)&&(pause)){
                detecterItemUtilise(&e,listeItem,&player1);
                printf("SOURIS");
            }
        }
        drawMap(game.renderer, mapOffsetX, mapOffsetY);
        drawTexture(player.texture, player.xTile * TILE_SIZE, player.yTile * TILE_SIZE, 64, 64);
        if(pause){
            affficherIventaire(game.renderer,Chiffre,listeItem);
        }
        if((itemDraw)||(timeDraw)){
            if(timeDraw ==0 ){
                timeDraw =1;
            }
            timeDraw = afficherItemObtenu(game.renderer,itemAfficher,timeDraw);
            printf("t = %d\n",timeDraw);
        }
        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }
    SDL_DestroyTexture(potion.t);
    SDL_DestroyTexture(potion2.t);
    SDL_DestroyTexture(atlasItem);
    SDL_DestroyTexture(Chiffre);
    SDL_Quit();
    return 0;
}