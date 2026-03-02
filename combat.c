#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

    SDL_Surface* texte=NULL;
    char* message = NULL; 
    SDL_Color couleur;

typedef struct {
    int hp;
    int max_hp;
    int attack;
    int speed;
    int xp;
} Fighter;

typedef enum {
    PLAYER_TURN,
    ENEMY_TURN,
    VICTORY,
    DEFEAT
} GameState;

int is_point_in_rect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void text(int x,int y,char*message,SDL_Color couleur){
    TTF_Init();

    TTF_Font* font = TTF_OpenFont("assets/ALGER.TTF", 100);

    SDL_Window *window = SDL_CreateWindow("Combat Tour par Tour",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640, 480,SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* texte=NULL;

    texte = TTF_RenderText_Blended(font, message,couleur);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, texte);
    SDL_FreeSurface(texte);

    SDL_GetRendererOutputSize(renderer, &x, &y);
    SDL_Rect position = {x/2.5, y/3, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);

    SDL_RenderCopy(renderer, texture, NULL, &position);

    TTF_CloseFont(font);
    TTF_Quit();
}


int main(int argc, char *argv[]){
    TTF_Init();

    TTF_Font* font = TTF_OpenFont("assets/ALGER.TTF", 100);

    char txt[][15]={"VICTORY","DEFEAT","RUN","FAIBLE","FORTE","INVENTAIRE"}; 

    SDL_Surface* texte=NULL;
    char* message = NULL; 
    SDL_Color couleur;

    int x,y;

    SDL_Init(SDL_INIT_VIDEO);
    srand(time(NULL));

    SDL_Window *window = SDL_CreateWindow("Combat Tour par Tour",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640, 480,SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Fighter player = {100, 100, 120, 100, 60};
    Fighter enemy  = {80,  80,  115, 20};

    GameState state = PLAYER_TURN;
    int running = 1;

    // Boutons
    SDL_Rect fuite = { 1400, 950, 425, 40 };
    SDL_Rect attack_btn = { 1625, 900, 200, 40 };
    SDL_Rect strong_attack_btn = { 1400, 900, 200, 40 };

    //barre
    SDL_Rect xp_possede = { 50, 960, player.xp *3, 10 };
    SDL_Rect xp_necessaire = { 50, 960, 100*3, 10 };
    
    while (running &&(player.hp>0 && enemy.hp>0)) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;

            if (e.type == SDL_MOUSEBUTTONDOWN && state == PLAYER_TURN) {
                int mx = e.button.x;
                int my = e.button.y;                

                if (is_point_in_rect(mx, my, attack_btn)) {
                    if(enemy.hp<50*(player.attack/100)){
                        enemy.hp=0;
                    }
                    else
                        enemy.hp -= 50*(player.attack/100);
                    state = (enemy.hp <= 0) ? VICTORY : ENEMY_TURN;
                }

                if (is_point_in_rect(mx, my, strong_attack_btn)) {
                    if(enemy.hp<30*(player.attack/100)){
                        enemy.hp=0;
                    }
                    else{
                        enemy.hp -= 30*(player.attack/100);
                    }
                    state = (enemy.hp <= 0) ? VICTORY : ENEMY_TURN;
                }


                if (is_point_in_rect(mx, my, fuite)) {
                    if(player.speed>enemy.speed){
                            SDL_DestroyRenderer(renderer);
                            SDL_DestroyWindow(window);
                            SDL_Quit();

                            return 0;
                    }
                    else{
                        state = ENEMY_TURN;
                    }
                }
            }

        }



        if (state == ENEMY_TURN) {
            SDL_Delay(500);
            switch(rand()%2) {
                case 0:player.hp = (player.hp<30*(enemy.attack/100)) ? 0 : player.hp-30*(enemy.attack/100);
                case 1:player.hp = (player.hp<50*(enemy.attack/100)) ? 0 : player.hp-50*(enemy.attack/100);
            }
                   
            state = (player.hp <= 0) ? DEFEAT : PLAYER_TURN;
        }

        // RENDER
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Barre de vie ennemi
        SDL_Rect enemy_hp = {50, 40, enemy.hp * 3, 50};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemy_hp);
        
        SDL_Rect player_hp = {50, 900, player.hp * 3, 50};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player_hp);

        // Bouton Attaquer
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        SDL_RenderFillRect(renderer, &attack_btn);

        text(1650,900,txt[3],(SDL_Color){0,0,0,255});

        // Barre de vie joueur
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player_hp);
        
        // Bouton Attaque forte
        SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
        SDL_RenderFillRect(renderer, &strong_attack_btn);

        //bouton fuite
        SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
        SDL_RenderFillRect(renderer, &fuite);

        //bouton xp necessaire
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &xp_necessaire);

        //bouton xp
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &xp_possede);

        SDL_Surface* image = SDL_LoadBMP("img/archer.bmp");

        SDL_Texture* monImage = SDL_CreateTextureFromSurface(renderer,image);  
        SDL_FreeSurface(image); //Équivalent du destroyTexture pour les surface, permet de libérer la mémoire quand on n'a plus besoin d'une surface

        SDL_Rect personnage = {100, 800, 0, 0};
        SDL_QueryTexture(monImage, NULL, NULL, &personnage.w, &personnage.h);

        SDL_RenderCopy(renderer, monImage, NULL, &personnage);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    switch(state){
        case DEFEAT:
            couleur = (SDL_Color){200, 0, 0, 255};
            message=txt[1];
            break;

        case VICTORY:
            couleur = (SDL_Color){0, 200, 0, 255};
            message=txt[0];
            break;        
    }

    texte = TTF_RenderText_Blended(font, message, couleur);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, texte);
    SDL_FreeSurface(texte);

    SDL_GetRendererOutputSize(renderer, &x, &y);
    SDL_Rect position = {x/2.5, y/3, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);

    SDL_RenderCopy(renderer, texture, NULL, &position);
    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
    TTF_Quit();


  

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

