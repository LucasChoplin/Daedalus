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
//gcc -o main main.c ./system/atlas.c ./system/map.c ./system/inventaire.c ./system/utilitaire.c ./system/text.c ./system/combat/combat_aff.c ./system/combat/combat.c ./system/combat/combat_attaque.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

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

    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    if((mode.w<SCREEN_WIDTH)||(mode.h<SCREEN_HEIGHT)){//si l'écran est trop petit la fenêtre est crée en fonction de sa taille 
        game.window = SDL_CreateWindow("Daedalus",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,mode.w,mode.h,windowFlags);
    }
    else{
        game.window = SDL_CreateWindow("Daedalus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    }

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
    SDL_RenderSetLogicalSize(game.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);//pour réajuster la taille des sprites 
    if(TTF_Init() < 0){
        fprintf(stderr, "Erreur TTF_Init : %s", TTF_GetError());
        return -1;
    }

    if(initText() < 0){
        return -1;
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_SetWindowIcon(game.window, IMG_Load("assets/logo_labyrinthe.png"));
    initAtlasMap(game.renderer);
    initAtlasItem(game.renderer);
    initAtlasMenu(game.renderer);
    initAtlasMob(game.renderer);
    initAtlasPerso(game.renderer);
    return 0;
}

////////////////////drawing///////////////////////  
void prepareScene(void){
    SDL_SetRenderDrawColor(game.renderer, 26, 23, 11, 255);
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
int checkMobCollision(int x, int y, Mob *mobs[], int nbMobs){
    for (int i = 0; i < nbMobs; i++) {
        if (currentMap->mapID == mobs[i]->mapID) {
            if (mobs[i]->xTile == x && mobs[i]->yTile == y) {
                return 1; //si mob
            }
        }
    }
    return 0; 
}

//nettoyage de la memoire
void cleanup(void){
    cleanupAtlasMap();
    cleanupAtlasItem();
    cleanupAtlasMenu();
    cleanupAtlasMob();
    cleanupAtlasPerso();
    cleanupText();
    cleanupMap();
    if(game.renderer != NULL)
        SDL_DestroyRenderer(game.renderer);
    if(game.window != NULL)
        SDL_DestroyWindow(game.window);
    SDL_Quit();
}

void setupBossParEtage(int etageActuel, int bossRoomID, int miniBossRoomID, Mob *miniBoss, Mob *boss){
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

void afficherCredits(SDL_Renderer * r){
    SDL_Color titleColor = {255, 255, 255, 255};
    SDL_Color textColor = {240, 230, 180, 255};
    SDL_Color skipColor = {100, 100, 100, 255};
    const int vitesse = 1; // px/frame
    int yBase = SCREEN_HEIGHT;
    int continuer = 1;
    const int espaceLigne = 70; //espacement entre les lignes de texte
    const int finCredits = -500; //position y où les credits se terminent

    while(continuer && yBase > finCredits){ 
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                continuer = 0;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_SPACE){
                    continuer = 0;
                }
            }
        }
    
        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
        SDL_RenderClear(r);

        drawText(r, getTitleFont(), "CREDITS", titleColor, SCREEN_WIDTH / 2 - 160, yBase);
        drawText(r, getDefaultFont(), "Developpeurs", textColor, SCREEN_WIDTH / 2 - 95, yBase + espaceLigne * 2);
        drawText(r, getDefaultFont(), "Patrick Leguillon", textColor, SCREEN_WIDTH / 2 - 110, yBase + espaceLigne * 3);
        drawText(r, getDefaultFont(), "Myriam Laaqira", textColor, SCREEN_WIDTH / 2 - 105, yBase + espaceLigne * 4);
        drawText(r, getDefaultFont(), "Lucas Choplin", textColor, SCREEN_WIDTH / 2 - 90, yBase + espaceLigne * 5);
        drawText(r, getDefaultFont(), "Graphismes : @hannilism", textColor, SCREEN_WIDTH / 2 - 155, yBase + espaceLigne * 7);
        drawText(r, getDefaultFont(), "Merci d'avoir joue a Daedalus !", textColor, SCREEN_WIDTH / 2 - 185, yBase + espaceLigne * 9);
        drawText(r, getDefaultFont(), "(Echap / Espace pour quitter)", textColor, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 40);

        SDL_RenderPresent(r);
        SDL_Delay(16);
        yBase -= vitesse; //pour faire défiler vers le haut
    }
}

int main(int argc, char *argv[]){

    Fighter fighter; 
    Mob miniBoss, boss, mob1, mob2, mob3, marchand;
    Bouton btnF; 
    int etageActuel = 1;
    int IDSalleTroc = -1;
    int menuMarchand = 0; //1 si le menu du marchand est ouvert
    int coffre = 0;//0 si pas ouvert, 1 si menu de loot et 2 si ouvert
    int coffreActif = 0; //indique si le coffre est actif
    int coffreMapID = -1; //indique la salle ou le coffre est
    int coffreX = -1;
    int coffreY = -1;
    int echelleActif = 0; //indique si le echelle d'etage est actif
    int echelleMapID = -1; //indique la salle ou le echelle est actif
    int echelleX = -1;
    int echelleY = -1;
    loot_t lootCoffre;
    loot_t stockMarchand = initStockMarchand();

    //initialise les pointeurs a NULL
    memset(&game, 0, sizeof(Game)); 
    memset(&player, 0, sizeof(Player));
    memset(&fighter, 0, sizeof(Fighter));
    memset(&mob1, 0, sizeof(Mob));
    memset(&mob2, 0, sizeof(Mob));
    memset(&mob3, 0, sizeof(Mob));
    memset(&miniBoss, 0, sizeof(Mob));
    memset(&boss, 0, sizeof(Mob));
    memset(&marchand, 0, sizeof(Mob));
    memset(&btnF, 0, sizeof(Bouton));

    initSDL();
    TTF_Font* btnfont = getDefaultFont();

    //-----------------------initialisation des variables de jeu --------------------------------
    fighter = (Fighter){.classeID=0, .hp=300, .max_hp=2000, .attack=120, .speed=100, .xp=0, .max_xp=100, .lvl=1, .pm_atk=8};    
    miniBoss = (Mob){.mapID=1, .spriteID=0, .xTile=5, .yTile=5, .hp=250, .max_hp=250, .attack=80, .speed=60};
    marchand = (Mob){.mapID=-1, .spriteID=1, .xTile=-1, .yTile=-1, .hp=1, .max_hp=1, .attack=0, .speed=0};
    boss = (Mob){.mapID=-1, .spriteID=1, .xTile=-1, .yTile=-1, .hp=500, .max_hp=500, .attack=100, .speed=50};
    mob1 = (Mob){.mapID=-1, .spriteID=0, .xTile=-1, .yTile=-1, .hp=60, .max_hp=60, .attack=50, .speed=30};
    mob2 = (Mob){.mapID=-1, .spriteID=0, .xTile=-1, .yTile=-1, .hp=80, .max_hp=60, .attack=70, .speed=40};
    mob3 = (Mob){.mapID=-1, .spriteID=0, .xTile=-1, .yTile=-1, .hp=100, .max_hp=100, .attack=30, .speed=90};
    Mob MobCombat[] = {miniBoss, boss, mob1, mob2, mob3};
    Mob* MobMap[] = {&miniBoss, &boss, &marchand};
    btnF = (Bouton){.couleurFond.r = 80, .couleurFond.g = 80, .couleurFond.b = 80, .couleurFond.a = 255, .couleurTexte.r = 255, .couleurTexte.g = 255, .couleurTexte.b = 255, .couleurTexte.a = 255, .texte = "F"};

    //apres l'init de TTF, on fini d'init le bouton d'interaction
    btnF.font = btnfont;
    btnF.rect.w = 30;  
    btnF.rect.h = 30;

    int mapPixelWidth = SALLE_WIDTH * TAILLE_TUILE;
    int mapPixelHeight = SALLE_HEIGHT * TAILLE_TUILE;
    int mapOffsetX = (SCREEN_WIDTH - mapPixelWidth) / 2;
    int mapOffsetY = (SCREEN_HEIGHT - mapPixelHeight) / 2;
    
    srand(time(NULL));
//----------------------------------------création des items -----------------------------------
    int pause = 0;/** sert à indiquer si le menu est ouvert 1 ou fermé 0 */
    int sortie = 0;/* variable servant à arrêter le programme */
    int menu = 0;/** variable servant à indiquer si un menu est ouvert */
    int itemObtenu[10]; 
    item_t potion;
    potion.f = soin50PV;
    item_t potion2;
    potion2.f = soin200PV;
    item_t potionEnergie;
    potionEnergie.f = NULL;
    item_t key;
    key.f = NULL;
    item_t corne;
    corne.f = augmenterAttaque;
    item_t anneau;
    anneau.f = augmenterPvmax;
    item_t sabot;
    sabot.f = augmenterVitesse;
    item_t couponReduction;
    couponReduction.f = NULL;
    item_t * listeItem[9];
    listeItem[0] = &potion;
    listeItem[1] = &potion2;
    listeItem[2] = &potionEnergie;
    listeItem[3] = &key;
    listeItem[4] = &corne;
    listeItem[5] = &anneau;
    listeItem[6] = &sabot;
    listeItem[7] = &couponReduction;
    listeItem[8] = NULL;
//----------------------chargement des variables de menu ---------------------------
    SDL_Rect destEchap = {1180,50,TAILLE_SPRITE/2,TAILLE_SPRITE/2};//position du bouton echap
//---------------------------------------------menu de départ
    SDL_Rect play = {(SCREEN_WIDTH-TAILLE_MENU)/2,SCREEN_HEIGHT/2.5-TAILLE_MENU,TAILLE_MENU,TAILLE_MENU/2};
    SDL_Rect continu = {(SCREEN_WIDTH-TAILLE_MENU)/2,SCREEN_HEIGHT/2-TAILLE_MENU,TAILLE_MENU,TAILLE_MENU/2};
    SDL_Rect quit = {(SCREEN_WIDTH - TAILLE_MENU) / 2,SCREEN_HEIGHT / 1.5 - TAILLE_MENU,TAILLE_MENU,TAILLE_MENU/2};

    const int menuRectH = TAILLE_MENU / 2;
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
                    cleanup();
                    return 0;
                }
                else if(detecterButtonClique(&event,&continu)){
                    sortie =1;
                }
            }
        }
        SDL_GetMouseState(&(sourisX),&(sourisY));
        if(detecterSourisDansRect(sourisX, sourisY, &play)){
            playF = getTileRect(1,ATLAS_BOUTON);
        }
        else{
            playF = getTileRect(0,ATLAS_BOUTON);     
        }
        if(detecterSourisDansRect(sourisX, sourisY, &quit)){
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
            if(detecterSourisDansRect(sourisX, sourisY, &continu)){
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
        
        int cardWidth = 200;
        int cardHeight = 400;
        int spacing = 40;
        int totalWidth = 3 * cardWidth + 2 * spacing;
        int startX = (SCREEN_WIDTH - totalWidth) / 2;
        int startY = (SCREEN_HEIGHT - cardHeight) / 2;
        int persoOffX = (cardWidth - TAILLE_SPRITE) / 2; //centrage horizontal du sprite dans la carte
        
        SDL_Rect DestGla = {startX + persoOffX, startY + 15, TAILLE_SPRITE, TAILLE_SPRITE};
        SDL_Rect DestArc = {startX + cardWidth + spacing + persoOffX, startY + 15, TAILLE_SPRITE, TAILLE_SPRITE};
        SDL_Rect DestLan = {startX + 2 * (cardWidth + spacing) + persoOffX, startY + 15, TAILLE_SPRITE, TAILLE_SPRITE};
        SDL_Rect glaF = getTileRect(1,ATLAS_PERSO);
        SDL_Rect archerF = getTileRect(0,ATLAS_PERSO);
        SDL_Rect lanF = getTileRect(2,ATLAS_PERSO);
        
        //fond pour chaque carte (aussi utilisé pour la détection du clic)
        SDL_Rect fondGla = {startX, startY, cardWidth, cardHeight};
        SDL_Rect fondArc = {startX + cardWidth + spacing, startY, cardWidth, cardHeight};
        SDL_Rect fondLan = {startX + 2 * (cardWidth + spacing), startY, cardWidth, cardHeight};
        
        TTF_Font* nameFont = getClassNameFont();
        TTF_Font* statsFont = getClassStatsFont();
        SDL_Color textColor = {240, 230, 180, 255};
        SDL_Color titleColor = {255, 255, 200, 255};
        
        int choixPerso = 0;
        while(choixPerso==0){//choix de la classe
            //on affiche le fond et les cartes
            SDL_SetRenderDrawColor(game.renderer, 30, 30, 30, 255);
            SDL_RenderClear(game.renderer);
            SDL_SetRenderDrawColor(game.renderer, 60, 60, 80, 255);
            SDL_RenderFillRect(game.renderer, &fondGla);
            SDL_RenderFillRect(game.renderer, &fondArc);
            SDL_RenderFillRect(game.renderer, &fondLan);
            SDL_RenderCopy(game.renderer, getAtlasPerso(), &glaF, &DestGla);
            SDL_RenderCopy(game.renderer, getAtlasPerso(), &archerF, &DestArc);
            SDL_RenderCopy(game.renderer, getAtlasPerso(), &lanF, &DestLan);

            drawText(game.renderer, titleFont, "CHOIX DE LA CLASSE", titleColor, 240, startY - 120);
            
            //GLADIATEUR
            drawText(game.renderer, nameFont, "GLADIATEUR", titleColor, startX + 8, startY + 200);
            drawText(game.renderer, statsFont, "PV: 200", textColor, startX + 10, startY + 240);
            drawText(game.renderer, statsFont, "ATQ: 120", textColor, startX + 10, startY + 280);
            drawText(game.renderer, statsFont, "VIT: 100", textColor, startX + 10, startY + 320);
            
            //ARCHER
            drawText(game.renderer, nameFont, "ARCHER", titleColor, startX + cardWidth + spacing + 25, startY + 200);
            drawText(game.renderer, statsFont, "PV: 100", textColor, startX + cardWidth + spacing + 10, startY + 240);
            drawText(game.renderer, statsFont, "ATQ: 220", textColor, startX + cardWidth + spacing + 10, startY + 280);
            drawText(game.renderer, statsFont, "VIT: 100", textColor, startX + cardWidth + spacing + 10, startY + 320);
            
            //LANCIER
            drawText(game.renderer, nameFont, "LANCIER", titleColor, startX + 2*(cardWidth + spacing) + 18, startY + 200);
            drawText(game.renderer, statsFont, "PV: 100", textColor, startX + 2*(cardWidth + spacing) + 10, startY + 240);
            drawText(game.renderer, statsFont, "ATQ: 120", textColor, startX + 2*(cardWidth + spacing) + 10, startY + 280);
            drawText(game.renderer, statsFont, "VIT: 200", textColor, startX + 2*(cardWidth + spacing) + 10, startY + 320);
            
            SDL_RenderPresent(game.renderer);
            
            SDL_Event event;
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_MOUSEBUTTONDOWN){
                    if(detecterButtonClique(&event,&fondGla)){
                        fprintf(f,"classeID=%d\n",GLADIATEUR);
                        fprintf(f,"pv_max=%d\n",GLADIATEUR_MAX_HP);
                        fprintf(f,"pv=%d\n",GLADIATEUR_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",GLADIATEUR_ATTACK);
                        fprintf(f,"stat_speed=%d\n",GLADIATEUR_SPEED);
                        choixPerso++;
                    }
                    else if(detecterButtonClique(&event,&fondArc)){
                        fprintf(f,"classeID=%d\n",ARCHER);
                        fprintf(f,"pv_max=%d\n",ARCHER_MAX_HP);
                        fprintf(f,"pv=%d\n",ARCHER_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",ARCHER_ATTACK);
                        fprintf(f,"stat_speed=%d\n",ARCHER_SPEED);
                        choixPerso++;
                    }
                    else if(detecterButtonClique(&event,&fondLan)){
                        fprintf(f,"classeID=%d\n",LANCIER);
                        fprintf(f,"pv_max=%d\n",LANCIER_MAX_HP);
                        fprintf(f,"pv=%d\n",LANCIER_MAX_HP);
                        fprintf(f,"stat_attaque=%d\n",LANCIER_ATTACK);
                        fprintf(f,"stat_speed=%d\n",LANCIER_SPEED);
                        choixPerso++;
                    }
                }
            }
        }
        fprintf(f,"xp=%d\n",0);
        fprintf(f,"niveau=%d\n",fighter.lvl);
        fprintf(f,"gold=%d\n",0);
        fprintf(f,"nb_potions=%d\n",0);
        fprintf(f,"nb_superpotions=%d\n",0);
        fprintf(f,"nb_PotionEnergie=%d\n",0);
        fprintf(f,"nb_clés=%d\n",0);
        fprintf(f,"nb_corne=%d\n",0);
        fprintf(f,"nb_anneau=%d\n",0);
        fprintf(f,"nb_sabot=%d\n",0);
        fprintf(f,"nb_couponReduction=%d\n",0);
        fprintf(f,"etage=%d\n",1);
        fclose(f);
    }
    f = fopen(FICHIER_DATA,"r");//chargement des données dans les variables locales 
    fscanf(f,"classeID=%d\n",&(fighter.classeID));
    fscanf(f,"pv_max=%d\n",&(fighter.max_hp));
    fscanf(f,"pv=%d\n",&(fighter.hp));
    fscanf(f,"stat_attaque=%d\n",&(fighter.attack));
    fscanf(f,"stat_speed=%d\n",&(fighter.speed)); 
    fscanf(f,"xp=%d\n",&(fighter.xp));
    fscanf(f,"niveau=%d\n",&(fighter.lvl));
    fscanf(f,"gold=%d\n",&(fighter.gold));
    fscanf(f,"nb_potions=%d\n",&(listeItem[0]->nb));
    fscanf(f,"nb_superpotions=%d\n",&(listeItem[1]->nb));
    fscanf(f,"nb_PotionEnergie=%d\n",&(listeItem[2]->nb));
    fscanf(f,"nb_clés=%d\n",&(listeItem[3]->nb));
    fscanf(f,"nb_corne=%d\n",&(listeItem[4]->nb));
    fscanf(f,"nb_anneau=%d\n",&(listeItem[5]->nb));
    fscanf(f,"nb_sabot=%d\n",&(listeItem[6]->nb));
    fscanf(f,"nb_couponReduction=%d\n",&(listeItem[7]->nb));
    /*switch(fighter.classeID){
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
            printf("ClasseID invalide dans le fichier de sauvegarde, classe par défaut attribuée\n");
            fighter.max_hp = 100;
    }*/
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
    initMapParEtage(etageActuel, &IDSalleBoss, &IDSalleMiniBoss, &IDSalleTroc);
    setupBossParEtage(etageActuel, IDSalleBoss, IDSalleMiniBoss, &miniBoss, &boss);
    if(IDSalleTroc >= 0){ 
        marchand.mapID = IDSalleTroc;
        marchand.xTile = SALLE_WIDTH / 2;
        marchand.yTile = SALLE_HEIGHT / 2 - 1;
    } else {
        marchand.mapID = -1;
    }
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
            if(menu==0 && menuMarchand == 0 && coffre != 1){ //si aucun menu n'est ouvert
                if((e.type == SDL_KEYDOWN)&&(e.key.keysym.sym == SDLK_ESCAPE)){//touche echap = arrêt du programme 
                    sortie = 1;
                }
                if(!pause && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){ //touche f pour interagir avec l'echelle ou le boss
                    int procheBossFinal = 0;
                    int procheMiniBoss = 0;
                    int procheEchelle = 0;

                    if(currentMap->mapID == bossFinal->mapID){
                        procheBossFinal = ((abs(player.xTile - bossFinal->xTile) + abs(player.yTile - bossFinal->yTile)) == 1); 
                    }
                    if(miniBossActif && currentMap->mapID == miniBossActif->mapID){
                        procheMiniBoss = ((abs(player.xTile - miniBossActif->xTile) + abs(player.yTile - miniBossActif->yTile)) == 1);
                    }
                    if(echelleActif && currentMap->mapID == echelleMapID){
                        procheEchelle = ((abs(player.xTile - echelleX) + abs(player.yTile - echelleY)) == 1);
                    }
                    int procheMarchand = (marchand.mapID >= 0 && currentMap->mapID == marchand.mapID) &&
                        ((abs(player.xTile - marchand.xTile) + abs(player.yTile - marchand.yTile)) == 1);
                    int procheCoffre = (currentMap->tiles[SALLE_HEIGHT / 2][SALLE_WIDTH / 2] == 11 && coffre != 2) &&
                        ((abs(player.xTile - (SALLE_WIDTH / 2)) + abs(player.yTile - (SALLE_HEIGHT / 2))) == 1);

                    if(procheEchelle){ //quand on passe a l'etage suivant
                        etageActuel++; 
                        coffre =0;
                        loot_t stockMarchand = initStockMarchand();
                        initMapParEtage(etageActuel, &IDSalleBoss, &IDSalleMiniBoss, &IDSalleTroc);
                        player.xTile = 9;
                        player.yTile = 7;
                        setupBossParEtage(etageActuel, IDSalleBoss, IDSalleMiniBoss, &miniBoss, &boss);
                        if(IDSalleTroc >= 0){
                            marchand.mapID = IDSalleTroc;
                            marchand.xTile = SALLE_WIDTH / 2;
                            marchand.yTile = SALLE_HEIGHT / 2 - 1;
                        } else {
                            marchand.mapID = -1;
                        }

                        echelleActif = 0;
                        echelleMapID = -1;
                        echelleX = -1;
                        echelleY = -1;

                        saveGameData(&fighter, listeItem, etageActuel);
                    }else if(procheMiniBoss){
                        lancerCombat(game.renderer, &fighter, miniBossActif, listeItem);
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
                    else if(procheMarchand){
                        if(e.type ==SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){
                            menuMarchand = 1;
                        }
                    }
                    else if(procheCoffre){
                        if(e.type ==SDL_KEYDOWN && e.key.keysym.sym == SDLK_f && coffre == 0 && key.nb >0){
                            lootCoffre = dropCoffre(listeItem,&fighter);
                            coffre = 1;
                            listeItem[3]->nb --; //enleve une clé
                        }
                    }
                    else if(procheBossFinal){ 
                        lancerCombat(game.renderer, &fighter, bossFinal, listeItem);
                        if(bossFinal->hp <= 0){
                            if(etageActuel < 3){
                                // avant l'etage 3, battre le miniboss fait apparaitre l'item pour continuer
                                echelleActif = 1;
                                echelleMapID = bossFinal->mapID;
                                echelleX = bossFinal->xTile;
                                echelleY = bossFinal->yTile;

                                bossFinal->mapID = -1; //enleve le boss de la map
                                bossFinal->xTile = -1;
                                bossFinal->yTile = -1;
                                bossFinal->hp = bossFinal->max_hp;
                            }else{
                                //boss battu: fin de partie
                                afficherCredits(game.renderer);
                                //affichage fin du jeu a implementer
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
                        if(!isWall(tile) && !(echelleActif && currentMap->mapID == echelleMapID && newX == echelleX && newY == echelleY) && !checkMobCollision(newX, newY, MobMap, 3)){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }else if(newX < 0){
                        changeSalle(GAUCHE, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        if(!isWall(tile) && !(echelleActif && currentMap->mapID == echelleMapID && newX == echelleX && newY == echelleY) && !checkMobCollision(newX, newY, MobMap, 3)){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }else if(newY >= SALLE_HEIGHT){
                        changeSalle(BAS, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        if(!isWall(tile) && !(echelleActif && currentMap->mapID == echelleMapID && newX == echelleX && newY == echelleY) && !checkMobCollision(newX, newY, MobMap, 3)){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }else if(newY < 0){
                        changeSalle(HAUT, &newX, &newY);
                        int tile = currentMap->tiles[newY][newX];
                        if(!isWall(tile) && !(echelleActif && currentMap->mapID == echelleMapID && newX == echelleX && newY == echelleY) && !checkMobCollision(newX, newY, MobMap, 3)){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }else{
                        int tile = currentMap->tiles[newY][newX];
                        if(!isWall(tile) && !(echelleActif && currentMap->mapID == echelleMapID && newX == echelleX && newY == echelleY) && !checkMobCollision(newX, newY, MobMap, 3)){
                            player.xTile = newX;
                            player.yTile = newY;
                            deplacement = 1;
                        }
                    }

                    if(deplacement && !pause && isCombatTile(currentMap->tiles[player.yTile][player.xTile])){
                        if(rand() % 100 < 20){ //20% de chance de lancer un combat sur une tile de combat
                            int randomMob = rand() % 3 + 2; //choisit un mob aléatoire parmi les 3 mobs lambda
                            Mob ennemiCombat = MobCombat[randomMob];
                            ennemiCombat.hp = ennemiCombat.max_hp;
                            lancerCombat(game.renderer, &fighter, &ennemiCombat, listeItem);
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
                        lootCoffre = dropCoffre(listeItem,&fighter);
                        coffre = 1;
                        //menu = dropItem(game.renderer,listeItem,itemObtenu,1,1,1,1,1,1);
                    }
                }
                if((e.type == SDL_MOUSEBUTTONDOWN)&&(pause)){
                    detecterItemUtilise(&e,listeItem,&fighter);
                }
            }
            else{
                if(e.type == SDL_MOUSEBUTTONDOWN && menu){
                    if(detecterButtonClique(&e,&destEchap)){
                        menu = 0;
                    }
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN && menuMarchand){
                    detecterAchat(&e,&fighter, listeItem,&stockMarchand);
                    if(detecterButtonClique(&e,&destEchap)){
                        menuMarchand = 0;
                    }
                }
                else if(e.type == SDL_MOUSEBUTTONDOWN && coffre == 1){
                    if (detecterButtonClique(&e,&destEchap)){
                        coffre = 2;
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
        if(marchand.mapID >= 0 && currentMap->mapID == marchand.mapID){
            drawMob(game.renderer, &marchand);
        }

        if(echelleActif && currentMap->mapID == echelleMapID){
            SDL_Rect srcItem = {256, 0, 64, 64};
            SDL_Rect destItem = {
                echelleX * TAILLE_TUILE + (TAILLE_TUILE - 64) / 2, echelleY * TAILLE_TUILE + (TAILLE_TUILE - 64) / 2, 64, 64
            };
            SDL_RenderCopy(game.renderer, getAtlasItem(), &srcItem, &destItem);
        }
        
        drawPlayer(game.renderer, &player);

        //dessine le bouton si a cote du boss ou de l'item d'etage
        int procheBossFinal = (currentMap->mapID == bossFinal->mapID) && ((abs(player.xTile - bossFinal->xTile) + abs(player.yTile - bossFinal->yTile)) == 1);
        int procheMiniBoss = miniBossActif && (currentMap->mapID == miniBossActif->mapID) && ((abs(player.xTile - miniBossActif->xTile) + abs(player.yTile - miniBossActif->yTile)) == 1);
        int procheEchelle = echelleActif && (currentMap->mapID == echelleMapID) && ((abs(player.xTile - echelleX) + abs(player.yTile - echelleY)) == 1);
        int procheMarchandRendu = (marchand.mapID >= 0 && currentMap->mapID == marchand.mapID) && ((abs(player.xTile - marchand.xTile) + abs(player.yTile - marchand.yTile)) == 1);
        int procheCofreRendu = (currentMap->tiles[SALLE_HEIGHT / 2][SALLE_WIDTH / 2] == 11 && coffre != 2) && ((abs(player.xTile - (SALLE_WIDTH / 2)) + abs(player.yTile - (SALLE_HEIGHT / 2))) == 1);
        if (procheBossFinal || procheMiniBoss || procheEchelle || procheMarchandRendu || procheCofreRendu){
            btnF.rect.x = player.xTile * TAILLE_TUILE + 50;
            btnF.rect.y = player.yTile * TAILLE_TUILE - 20;
            drawButton(game.renderer, &btnF);
        }
        if(pause){
            afficherInventaire(game.renderer,listeItem,&fighter);
        }
        if(menu){
            //afficherItemObtenu(game.renderer,menu,itemObtenu);
        }
        if(coffre==1){
            afficherItemObtenu(game.renderer,&lootCoffre);
        }
        if(menuMarchand){
            afficherMagasin(game.renderer, &fighter,listeItem,&stockMarchand);
        }
        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }
    saveGameData(&fighter, listeItem, etageActuel);
    cleanup();
    return 0;
}