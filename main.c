#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

    if(initText() < 0){
        return -1;
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_SetWindowIcon(game.window, IMG_Load("assets/logo_labyrinthe.png"));
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

//evite collision avec les mobs
int checkMobCollision(int x, int y , Mob mob){
    if (currentMap->mapID == mob.mapID) {
        if (mob.xTile == x && mob.yTile == y) {
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
    cleanupText();
    cleanupMap();
    if(game.renderer != NULL)
        SDL_DestroyRenderer(game.renderer);
    if(game.window != NULL)
        SDL_DestroyWindow(game.window);
    SDL_Quit();
}

void saveGameData(Fighter *fighter, item_t *listeItem[], int etageActuel){
    FILE *f = fopen(FICHIER_DATA, "w");
    if(!f){
        return;
    }

    fprintf(f,"classeID=%d\n",fighter->classeID);
    fprintf(f,"pv=%d\n",fighter->hp);
    fprintf(f,"stat_attaque=%d\n",fighter->attack);
    fprintf(f,"stat_speed=%d\n",fighter->speed);
    fprintf(f,"nb_potions=%d\n",listeItem[0]->nb);
    fprintf(f,"nb_superpotions=%d\n",listeItem[1]->nb);
    fprintf(f,"nb_clés=%d\n",listeItem[2]->nb);
    fprintf(f,"etage=%d\n",etageActuel);
    fclose(f);
}

void setupBossForFloor(int etageActuel, int bossRoomID, int miniBossRoomID, Mob *miniBoss, Mob *boss){
    Mob *bossActif = (etageActuel >= 3) ? boss : miniBoss;
    Mob *bossInactif = (etageActuel >= 3) ? miniBoss : boss;

    bossActif->mapID = bossRoomID;
    bossActif->xTile = SALLE_WIDTH / 2;
    bossActif->yTile = SALLE_HEIGHT / 2;
    bossActif->hp = bossActif->max_hp;

    if(etageActuel >= 3){
        bossInactif->mapID = miniBossRoomID;
        bossInactif->xTile = SALLE_WIDTH / 2;
        bossInactif->yTile = SALLE_HEIGHT / 2;
        bossInactif->hp = bossInactif->max_hp;
    }
    else{
        bossInactif->mapID = -1;
        bossInactif->xTile = -1;
        bossInactif->yTile = -1;
    }
}

int main(int argc, char *argv[]){

    Fighter fighter; 
    Mob miniBoss, boss, mob1, mob2, mob3;
    Bouton btnF; 
    int etageActuel = 1;
    int portailActif = 0; //indique si le portail d'etage est actif
    int portailMapID = -1; //indique la salle ou le portail est actif
    int portailX = -1;
    int portailY = -1;

    //initialise les pointeurs a NULL
    memset(&game, 0, sizeof(Game)); 
    memset(&player, 0, sizeof(Player));
    memset(&fighter, 0, sizeof(Fighter));
    memset(&mob1, 0, sizeof(Mob));
    memset(&mob2, 0, sizeof(Mob));
    memset(&mob3, 0, sizeof(Mob));
    memset(&miniBoss, 0, sizeof(Mob));
    memset(&boss, 0, sizeof(Mob));
    memset(&btnF, 0, sizeof(Bouton));

    initSDL();
    TTF_Font* btnfont = getDefaultFont();

    //-----------------------initialisation des variables de jeu --------------------------------
    fighter = (Fighter){.classeID=0, .hp=300, .max_hp=2000, .attack=120, .speed=100};
    miniBoss = (Mob){.mapID=1, .spriteID=0, .xTile=5, .yTile=5, .hp=30, .max_hp=20, .attack=80, .speed=60};
    boss = (Mob){.mapID=-1, .spriteID=1, .xTile=-1, .yTile=-1, .hp=30, .max_hp=15, .attack=100, .speed=50};
    mob1 = (Mob){.mapID=-1, .spriteID=0, .xTile=-1, .yTile=-1, .hp=60, .max_hp=6, .attack=50, .speed=30};
    mob2 = (Mob){.mapID=-1, .spriteID=0, .xTile=-1, .yTile=-1, .hp=80, .max_hp=8, .attack=70, .speed=40};
    mob3 = (Mob){.mapID=-1, .spriteID=0, .xTile=-1, .yTile=-1, .hp=100, .max_hp=10, .attack=30, .speed=90};
    Mob Mobs[] = {miniBoss, boss, mob1, mob2, mob3};
    btnF = (Bouton){.couleurFond.r = 80, .couleurFond.g = 80, .couleurFond.b = 80, .couleurFond.a = 255, .couleurTexte.r = 255, .couleurTexte.g = 255, .couleurTexte.b = 255, .couleurTexte.a = 255, .texte = "F"};

    //apres l'init de TTF, on fini d'init le bouton d'interaction
    btnF.font = btnfont;
    btnF.rect.w = 30;  //bouton carre
    btnF.rect.h = 30;

    int mapPixelWidth = SALLE_WIDTH * TILE_SIZE;
    int mapPixelHeight = SALLE_HEIGHT * TILE_SIZE;
    int mapOffsetX = (SCREEN_WIDTH - mapPixelWidth) / 2;
    int mapOffsetY = (SCREEN_HEIGHT - mapPixelHeight) / 2;
    
    srand(time(NULL));
//----------------------------------------création des items -----------------------------------
    int pause = 0;/** sert à indiquer si le menu est ouvert 1 ou fermé 0 */
    int sortie = 0;/* variable servant à arrêter le programme */
    int menu = 0;/** variable servant à indiquer si un menu est ouvert */
    int itemObtenu[10]; 
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
    SDL_Rect play = {(SCREEN_WIDTH-TAILLE_MENU)/2,SCREEN_HEIGHT/2.5-TAILLE_MENU,TAILLE_MENU,TAILLE_MENU};
    SDL_Rect continu = {(SCREEN_WIDTH-TAILLE_MENU)/2,SCREEN_HEIGHT/2-TAILLE_MENU,TAILLE_MENU,TAILLE_MENU};
    SDL_Rect quit = {(SCREEN_WIDTH-TAILLE_MENU)/2,SCREEN_HEIGHT/1.5-TAILLE_MENU,TAILLE_MENU,TAILLE_MENU};
    TTF_Font* titleFont = getTitleFont();
    SDL_Texture* titleTexture = NULL;
    SDL_Rect titleRect = {0, 0, 0, 0};

    if(titleFont != NULL){
        SDL_Color titleColor = {240, 230, 180, 255};
        SDL_Surface* titleSurface = TTF_RenderUTF8_Blended(titleFont, "DAEDALUS", titleColor);
        if(titleSurface != NULL){
            titleTexture = SDL_CreateTextureFromSurface(game.renderer, titleSurface);
            titleRect.w = titleSurface->w;
            titleRect.h = titleSurface->h;
            titleRect.x = (SCREEN_WIDTH - titleRect.w) / 2;
            titleRect.y = play.y - titleRect.h - 30;
            if(titleRect.y < 20){
                titleRect.y = 20;
            }
            SDL_FreeSurface(titleSurface);
        }
    }

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
                    if(titleTexture != NULL)
                        SDL_DestroyTexture(titleTexture);
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
        if(titleTexture != NULL){
            SDL_RenderCopy(game.renderer, titleTexture, NULL, &titleRect);
        }
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
    if(titleTexture != NULL)
        SDL_DestroyTexture(titleTexture);
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
                        fprintf(f,"pv=%d\n",GLADIATEUR_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",GLADIATEUR_ATTACK);
                        fprintf(f,"stat_speed=%d\n",GLADIATEUR_SPEED);
                        choixPerso++;
                    }
                    else if(detecterButtonClique(&event,&DestArc)){
                        fprintf(f,"classeID=%d\n",ARCHER);
                        fprintf(f,"pv=%d\n",ARCHER_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",ARCHER_ATTACK);
                        fprintf(f,"stat_speed=%d\n",ARCHER_SPEED);
                        choixPerso++;
                    }
                    else if(detecterButtonClique(&event,&DestLan)){
                        fprintf(f,"classeID=%d\n",LANCIER);
                        fprintf(f,"pv=%d\n",LANCIER_MAX_HP);
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
        fprintf(f,"etage=%d\n",1);
        fclose(f);
    }
    f = fopen(FICHIER_DATA,"r");//chargement des données dans les variables locales 
    fscanf(f,"classeID=%d\n",&(fighter.classeID));
    fscanf(f,"pv=%d\n",&(fighter.hp));
    fscanf(f,"stat_attaque=%d\n",&(fighter.attack));
    fscanf(f,"stat_speed=%d\n",&(fighter.speed)); 
    fscanf(f,"nb_potions=%d\n",&(listeItem[0]->nb));
    fscanf(f,"nb_superpotions=%d\n",&(listeItem[1]->nb));
    fscanf(f,"nb_clés=%d\n",&(listeItem[2]->nb));
    switch(fighter.classeID){
        case GLADIATEUR:
            fighter.max_hp = GLADIATEUR_MAX_HP;
            break;
        case ARCHER:
            fighter.max_hp = ARCHER_MAX_HP;
            break;
        case LANCIER:
            fighter.max_hp = LANCIER_MAX_HP;
            break;
        default:
            fighter.max_hp = 100; //si classeID invalide
    }
    if(fscanf(f,"etage=%d\n",&(etageActuel)) != 1){
        etageActuel = 1;
    }
    if(etageActuel < 1){
        etageActuel = 1;
    }
    fclose(f);   
    sortie = 0;

    int IDSalleBoss = -1;
    int IDSalleMiniBoss = -1;
    initMapParEtage(etageActuel, &IDSalleBoss, &IDSalleMiniBoss);
    setupBossForFloor(etageActuel, IDSalleBoss, IDSalleMiniBoss, &miniBoss, &boss);
//----------------------------------------------------------------------------------------------
    //position de depart du player
    player.xTile = 9;
    player.yTile = 7;
    player.texture = getAtlasPerso();
    player.spriteID = fighter.classeID;
    atexit(cleanup); //nettoyera la memoire à la fermeture du programme
    ////////////////////////
    
    while(sortie==0){
        prepareScene();
        SDL_Event e;
        Mob *bossFinal = (etageActuel >= 3) ? &boss : &miniBoss;
        Mob *miniBossActif = (etageActuel >= 3) ? &miniBoss : NULL;
        
        //gestion des evenements
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                saveGameData(&fighter, listeItem, etageActuel);
                sortie = 1;
            }
            if(menu==0){
                if((e.type == SDL_KEYDOWN)&&(e.key.keysym.sym == SDLK_ESCAPE)){//touche echap = arrêt du programme 
                    SDL_DestroyTexture(Chiffre);
                    sortie = 1;
                }
                if(!pause && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){ //touche f pour interagir avec le portail ou le boss
                    int procheBossFinal = 0;
                    int procheMiniBoss = 0;
                    int prochePortail = 0;

                    if(currentMap->mapID == bossFinal->mapID){
                        procheBossFinal = ((abs(player.xTile - bossFinal->xTile) + abs(player.yTile - bossFinal->yTile)) == 1); 
                    }
                    if(miniBossActif && currentMap->mapID == miniBossActif->mapID){
                        procheMiniBoss = ((abs(player.xTile - miniBossActif->xTile) + abs(player.yTile - miniBossActif->yTile)) == 1);
                    }
                    if(portailActif && currentMap->mapID == portailMapID){
                        prochePortail = ((abs(player.xTile - portailX) + abs(player.yTile - portailY)) == 1);
                    }

                    if(prochePortail){ //quand on passe a l'etage suivant
                        etageActuel++; 
                        initMapParEtage(etageActuel, &IDSalleBoss, &IDSalleMiniBoss);
                        player.xTile = 9;
                        player.yTile = 7;
                        setupBossForFloor(etageActuel, IDSalleBoss, IDSalleMiniBoss, &miniBoss, &boss);

                        portailActif = 0;
                        portailMapID = -1;
                        portailX = -1;
                        portailY = -1;

                        saveGameData(&fighter, listeItem, etageActuel);
                    }else if(procheMiniBoss){
                        lancerCombat(game.renderer, &fighter, miniBossActif, listeItem, Chiffre);
                        if(miniBossActif->hp <= 0){
                            //miniBoss battu: disparition sur cet etage
                            miniBossActif->mapID = -1;
                            miniBossActif->xTile = -1;
                            miniBossActif->yTile = -1;
                            miniBossActif->hp = miniBossActif->max_hp;

                            if(etageActuel == 3){
                                //ouvre les portes du boss final
                            }
                        }
                    }
                    else if(procheBossFinal){ 
                        lancerCombat(game.renderer, &fighter, bossFinal, listeItem, Chiffre);
                        if(bossFinal->hp <= 0){
                            if(etageActuel < 3){
                                // avant l'etage 3, battre le miniboss fait apparaitre l'item pour continuer
                                portailActif = 1;
                                portailMapID = bossFinal->mapID;
                                portailX = bossFinal->xTile;
                                portailY = bossFinal->yTile;

                                bossFinal->mapID = -1; //enleve le boss de la map
                                bossFinal->xTile = -1;
                                bossFinal->yTile = -1;
                                bossFinal->hp = bossFinal->max_hp;
                            }else{
                                // boss battu: fin de partie
                                saveGameData(&fighter, listeItem, etageActuel);
                                sortie = 1;
                            }
                        }
                    }
                }
                if(e.type == SDL_KEYDOWN){
                    int newX = player.xTile;
                    int newY = player.yTile;
                    int toucheDeplacement = 0;
                    int deplacement = 0;

                    switch(e.key.keysym.sym){
                        case SDLK_UP:
                            toucheDeplacement = 1;
                            newY --;
                            break;
                        case SDLK_DOWN:
                            toucheDeplacement = 1;
                            newY ++;
                            break;
                        case SDLK_LEFT:
                            toucheDeplacement = 1;
                            player.facing = GAUCHE;
                            newX --;
                            break;
                        case SDLK_RIGHT:
                            toucheDeplacement = 1;
                            player.facing = DROITE;
                            newX ++;
                            break;
                    }

                    if(!toucheDeplacement){
                        continue;
                    }

                    //on verifie les collisions et le changement de salle
                    if(newX >= SALLE_WIDTH){
                        changeSalle(DROITE, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        int collisionBossFinal = checkMobCollision(newX, newY, *bossFinal);
                        int collisionMiniBoss = (miniBossActif != NULL) ? checkMobCollision(newX, newY, *miniBossActif) : 0;
                        if(!isWall(tile) && !collisionBossFinal && !collisionMiniBoss){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }else if(newX < 0){
                        changeSalle(GAUCHE, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        int collisionBossFinal = checkMobCollision(newX, newY, *bossFinal);
                        int collisionMiniBoss = (miniBossActif != NULL) ? checkMobCollision(newX, newY, *miniBossActif) : 0;
                        if(!isWall(tile) && !collisionBossFinal && !collisionMiniBoss){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }else if(newY >= SALLE_HEIGHT){
                        changeSalle(BAS, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        int collisionBossFinal = checkMobCollision(newX, newY, *bossFinal);
                        int collisionMiniBoss = (miniBossActif != NULL) ? checkMobCollision(newX, newY, *miniBossActif) : 0;
                        if(!isWall(tile) && !collisionBossFinal && !collisionMiniBoss){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }else if(newY < 0){
                        changeSalle(HAUT, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        int collisionBossFinal = checkMobCollision(newX, newY, *bossFinal);
                        int collisionMiniBoss = (miniBossActif != NULL) ? checkMobCollision(newX, newY, *miniBossActif) : 0;
                        if(!isWall(tile) && !collisionBossFinal && !collisionMiniBoss){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }else{
                        int tile = currentMap->tiles[newY][newX];
                        int collisionBossFinal = checkMobCollision(newX, newY, *bossFinal);
                        int collisionMiniBoss = (miniBossActif != NULL) ? checkMobCollision(newX, newY, *miniBossActif) : 0;
                        if(!isWall(tile) && !collisionBossFinal && !collisionMiniBoss){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }

                    if(deplacement && !pause && isCombatTile(currentMap->tiles[player.yTile][player.xTile])){
                        if(rand() % 100 < 20){ //20% de chance de lancer un combat sur une tile de combat
                            int randomMob = rand() % 3 + 2; //choisit un mob aléatoire parmi les 3 mobs lambda
                            Mob ennemiCombat = Mobs[randomMob];
                            ennemiCombat.hp = ennemiCombat.max_hp;
                            lancerCombat(game.renderer, &fighter, &ennemiCombat, listeItem, Chiffre);
                        }
                    }
                }
                if(e.type == SDL_KEYUP){
                    if(e.key.keysym.sym == SDLK_TAB){//si la touche est tab est presse
                        if(pause==0){
                            pause = 1;
                        }
                        else{
                            pause = 0;
                        }
                    }
                    if(e.key.keysym.sym == SDLK_a){//SERT a tester les drops de mob
                        drawPlayer(game.renderer, &player);
                        menu = dropItem(game.renderer,listeItem,itemObtenu,1,1,1,1,1,1);
                    }
                }
                if((e.type == SDL_MOUSEBUTTONDOWN)&&(pause)){
                    detecterItemUtilise(&e,listeItem,&fighter);
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
        if (currentMap->mapID == bossFinal->mapID) {
            drawMob(game.renderer, bossFinal);
        }
        if (miniBossActif && currentMap->mapID == miniBossActif->mapID) {
            drawMob(game.renderer, miniBossActif);
        }

        if(portailActif && currentMap->mapID == portailMapID){
            SDL_Rect srcItem = getTileRect(2, ATLAS_ITEM);
            SDL_Rect destItem = {portailX * TILE_SIZE, portailY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderCopy(game.renderer, getAtlasItem(), &srcItem, &destItem);
        }
        
        drawPlayer(game.renderer, &player);

        //dessine le bouton si a cote du boss ou de l'item d'etage
        int procheBossFinal = (currentMap->mapID == bossFinal->mapID) &&
            ((abs(player.xTile - bossFinal->xTile) + abs(player.yTile - bossFinal->yTile)) == 1);
        int procheMiniBoss = miniBossActif && (currentMap->mapID == miniBossActif->mapID) &&
            ((abs(player.xTile - miniBossActif->xTile) + abs(player.yTile - miniBossActif->yTile)) == 1);
        int prochePortail = portailActif && (currentMap->mapID == portailMapID) &&
            ((abs(player.xTile - portailX) + abs(player.yTile - portailY)) == 1);
        if (procheBossFinal || procheMiniBoss || prochePortail){
            btnF.rect.x = player.xTile * TILE_SIZE + 50;
            btnF.rect.y = player.yTile * TILE_SIZE - 20;
            drawButton(game.renderer, &btnF);
        }
        if(pause){
            afficherInventaire(game.renderer,listeItem,&fighter);
        }
        if(menu){
            afficherItemObtenu(game.renderer,menu,itemObtenu);
        }
        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }
    saveGameData(&fighter, listeItem, etageActuel);
    SDL_DestroyTexture(Chiffre);
    cleanup();
    return 0;
}