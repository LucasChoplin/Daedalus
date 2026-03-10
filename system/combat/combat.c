#include "combat.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960

int is_point_in_rect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}


int lancerCombat(SDL_Renderer *renderer){

    int x=10,y=10;

    TTF_Init();

    TTF_Font* font = TTF_OpenFont("times.ttf", 100);

    Fighter player = {100, 100, 120, 100, 60};
    Fighter enemy  = {80,  80,  115, 20};

    GameState state = PLAYER_TURN;
    int running = 1;

    // Boutons
    SDL_Rect fuite = { 800, 900, 425, 40 };
    SDL_Rect attack_btn = { 800, 850, 200, 40 };
    SDL_Rect strong_attack_btn = {1025, 850, 200, 40 };
    SDL_Rect inventaire = { 800, 800, 425, 40 };

    //barre
    SDL_Rect xp_possede = { 50, 935, player.xp *3, 10 };
    SDL_Rect xp_necessaire = { 50, 935, 100*3, 10 };

    //affichage de l'image
    SDL_Surface* image = SDL_LoadBMP("Img/archer.bmp");

    SDL_Texture* monImage = SDL_CreateTextureFromSurface(renderer, image);  
    SDL_FreeSurface(image);

    SDL_Rect personnage = {150, 800, 0, 0};
    SDL_QueryTexture(monImage, NULL, NULL, &personnage.w, &personnage.h);

    SDL_Rect ennemy = {1100, 100, 0, 0};
    SDL_QueryTexture(monImage, NULL, NULL, &ennemy.w, &ennemy.h);

    
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
                        SDL_DestroyTexture(monImage);
                        TTF_CloseFont(font);
                        TTF_Quit();
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
                case 0:player.hp = (player.hp<30*(enemy.attack/100)) ? 0 : player.hp-30*(enemy.attack/100);break;
                case 1:player.hp = (player.hp<50*(enemy.attack/100)) ? 0 : player.hp-50*(enemy.attack/100);break;
            }
                   
            state = (player.hp <= 0) ? DEFEAT : PLAYER_TURN;
        }

        // RENDER
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Barre de vie ennemi
        SDL_Rect enemy_hp = {1000, 40, enemy.hp * 3, 30};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemy_hp);
        
        SDL_Rect player_hp = {50, 900, player.hp * 3, 30};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player_hp);

        // Bouton Attaquer
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        SDL_RenderFillRect(renderer, &attack_btn);

        // Barre de vie joueur
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player_hp);
        
        // Bouton Attaque forte
        SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
        SDL_RenderFillRect(renderer, &strong_attack_btn);

        //bouton fuite
        SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
        SDL_RenderFillRect(renderer, &fuite);
        
        //bouton inventaire
        SDL_SetRenderDrawColor(renderer, 250, 0, 250, 255);
        SDL_RenderFillRect(renderer, &inventaire);

        //bouton xp necessaire
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &xp_necessaire);

        //bouton xp
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &xp_possede);

        SDL_RenderCopy(renderer, monImage, NULL, &personnage);
        SDL_RenderCopy(renderer, monImage, NULL, &ennemy);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    SDL_Delay(2000);
    SDL_DestroyTexture(monImage);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    SDL_Surface* texte=NULL;
    char* message = NULL; 
    SDL_Color couleur;

    switch(state){
        case DEFEAT:
            couleur = (SDL_Color){200, 0, 0, 255};
            message="DEFEAT";
            break;

        case VICTORY:
            couleur = (SDL_Color){0, 200, 0, 255};
            message="VICTORY";
            break;        
    }

    texte = TTF_RenderText_Blended(font, message, couleur);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, texte);
    SDL_FreeSurface(texte);

    SDL_GetRendererOutputSize(renderer, &x, &y);
    SDL_Rect position = {500, 500, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);

    SDL_RenderCopy(renderer, texture, NULL, &position);
    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
    TTF_Quit();
    return 0;
}

