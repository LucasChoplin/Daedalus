#include <stdio.h>
#include <SDL2/SDL.h>
/** \file Inventaire.c
    \brief Programme pour afficher l'inventaire quand tab est pressé.
    \author Patrick Leguillon
    \version 1.0
    \date 20 février 2026
*/

#define TAILLE_CHIFFRE 32
#define TAILLE_ITEM 32
#define NB_ITEM 2

/** \brief structure pour les items  */
typedef struct{
    int nb;/**< nom de l'image de l'item*/
    SDL_Texture * t;/**< quantité de l'item */
} item_t;

/** \brief fonction pour charger les images danns une texture (enlève la couleur blanche de l'image aussi (255.255.255))
    \param nom nom du fichier bpm à charger 
    \param r pointeur vers le moteur de rendu 
    \param t pointeur vers le pointeur de Texture où stockée la Texture
 */
void chargerImage(char nom[],SDL_Renderer * r,SDL_Texture ** t){
    SDL_Surface * s = SDL_LoadBMP(nom);
    SDL_SetColorKey(s, SDL_TRUE, SDL_MapRGB(s->format, 255, 255, 255));
    (*t) = SDL_CreateTextureFromSurface(r,s);
    SDL_FreeSurface(s);
}

/** \brief indique si un clic de souris touche une texture 
    \param e pointeur sur l'event de la souris enclenché sert à récupérer le x et y de la souris 
    \param r pointeur sur le RECT de la texture pour récupérer son x y w et h
    \return 1 si la souris touche la texture et 0 si elle ne touvhe pas 
*/
int detecterButtonClique(SDL_Event * e,SDL_Rect * r){
    SDL_Point p = {e->button.x,e->button.y};//je crée un point à partir des coordonnées de la souris 
    return SDL_PointInRect(&p,r); 
    //j'utilise la fonction PointInRect qui vérifie si p et dans le rectangle de l'image pour Rect r
}


/** \brief fonction qui affiche un chiffre 
    \param r pointeur vers le moteur de rendu 
    \param t pointeur vers la texture de chiffre de 32px * 32 px
    \param nb chiffre à afficher compris entre 0 et 9 
    \param d pointeur vers le Rect contenant le x et y ou afficher le chiffre 
*/
void afficherChiffre(SDL_Renderer * r,SDL_Texture * t,int nb,SDL_Rect * d){
    d->w = 32;
    d->h = 32;
    SDL_Rect chif = {nb*32,0,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    SDL_RenderCopy(r, t,&chif,d);
}

/** \brief fonction qui affiche l'inventaire  
    \param r pointeur vers le moteur de rendu 
    \param t pointeur vers la texture des chiffres 
    \param l liste de pointeurs vers les item_t
*/
void affficherIventaire(SDL_Renderer * r, SDL_Texture * t, item_t * l[]){
    int x = 100;
    SDL_Rect item = {100,100,TAILLE_ITEM,TAILLE_ITEM};
    SDL_Rect c = {100,120,TAILLE_CHIFFRE,TAILLE_CHIFFRE};
    for(int i =0; i<NB_ITEM;i++){
        if(l[i]->nb>0){
            SDL_RenderCopy(r, l[i]->t, NULL, &item);
            afficherChiffre(r,t,l[i]->nb,&c);
            x+=100;
            item.x = x;
            c.x = x;
        }
    }
}

/** \brief fonction qui détecte losqu'on appuie sur un item et diminue sa quantité 
    \param event pointeur vers l'événement souris cliqué 
    \param l liste de pointeurs vers les item_t
 */
void detecterItemUtilise(SDL_Event * event, item_t * l[]){
    int x = 100;
    SDL_Rect item = {100,100,TAILLE_ITEM,TAILLE_ITEM};
    for(int i = 0;i < NB_ITEM;i++){
        if(l[i]->nb>0){
            if(detecterButtonClique(event,&item)){
                l[i]->nb--;
            }
            x+=100;
            item.x = x;
        }
    }
}

/** \brief fonction qui libère les textures chargées des items 
    \param l liste de pointeurs vers les item_t
 */
void FreeTextureItem(item_t * l[]){
    for(int i = 0; i < NB_ITEM;i++){
        SDL_DestroyTexture(l[i]->t);
    }
}




int main(int argc, char *argv[]){
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);//sert à récupérer la taille de l'écran actuelle 
    SDL_Window *window = SDL_CreateWindow( "Test TAB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h,0);
    //SDL_Window *window = SDL_CreateWindow( "Test TAB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400,0);
    //ouverture d'une fenêtre 
    SDL_Renderer * renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    int pause = 0;/** sert à indiquer si le menu est ouvert 1 ou fermé 0 */
    int sortie = 0;/* variable servant à arrêter le programme */
    SDL_Event event;/** variable servant à stocker les evénements détectés par SDL_PollEvent */
//----------------------initialisation des items ------------------------------------
    item_t potion;
    potion.nb = 1;
    chargerImage("Img/potion.bmp",renderer,&potion.t);
    item_t potion2;
    potion2.nb = 5;
    chargerImage("Img/potion2.bmp",renderer,&potion2.t);
    item_t * listeItem[3];
    listeItem[0] = &potion;
    listeItem[1] = &potion2;

//----------------------Chargement d'image ------------------------------------------
    SDL_Surface * surface = SDL_GetWindowSurface(window);
    SDL_Texture * Chiffre;
    chargerImage("Img/chiffreTest.bmp",renderer,&Chiffre);
    SDL_Rect srcDigits[10]; 
    int digitWidth = 32; 
    int digitHeight = 32; // hauteur d’un chiffre 
    for (int i = 0; i < 10; i++) { 
        srcDigits[i].x = i * digitWidth; 
        srcDigits[i].y = 0; 
        srcDigits[i].w = digitWidth; 
        srcDigits[i].h = digitHeight; 
    }
//--------------------------------------------------------------------------------
    while(sortie==0){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYUP){//si une touche est relaché
                if(event.key.keysym.sym == SDLK_TAB){//si la touche est tab 
                    printf("Tab détecté");
                    if(pause==0){
                        pause = 1;
                    }
                    else{
                        pause = 0;
                    }
                }
                if(event.key.keysym.sym==SDLK_ESCAPE){//touche echap = arrêt du programme 
                    sortie = 1;
                }
            }
            if((event.type == SDL_MOUSEBUTTONDOWN)&&(pause)){
                detecterItemUtilise(&event,listeItem);
                printf("SOURIS");
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);
        if(pause){
            affficherIventaire(renderer,Chiffre,listeItem);
        }
        SDL_RenderPresent(renderer);
    }

//fin du programme libérez les objets chargés 
    SDL_DestroyTexture(potion.t);
    SDL_DestroyTexture(potion2.t);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window); 
    SDL_Quit(); 
    return 0;
}