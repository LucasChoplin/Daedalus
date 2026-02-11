#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_ttf.h>

typedef struct {
    int hp;
    int max_hp;
    int attack;
    int speed;
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

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(NULL));

    SDL_Window *window = SDL_CreateWindow("Combat Tour par Tour",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640, 480,SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Fighter player = {100, 100, 120, 100};
    Fighter enemy  = {80,  80,  115, 20};

    GameState state = PLAYER_TURN;
    int running = 1;

    // Boutons
    SDL_Rect attack = { 500, 350, 100, 100  };
    SDL_Rect fuite = { 50, 400, 425, 40 };

    while (running &&(player.hp>0 && enemy.hp>0)) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;

            if (e.type == SDL_MOUSEBUTTONDOWN && state == PLAYER_TURN) {
                int mx = e.button.x;
                int my = e.button.y;

                if(is_point_in_rect(mx, my, attack)){
                    // Bouton Attaquer
                    SDL_Rect attack_btn = { 275, 350, 200, 40 };
                    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
                    SDL_RenderFillRect(renderer, &attack_btn);
                    // Bouton Attaque forte
                    SDL_Rect strong_attack_btn = { 50, 350, 200, 40 };
                    SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
                    SDL_RenderFillRect(renderer, &strong_attack_btn);

                    SDL_Delay(1000);
                

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
                }

                if (is_point_in_rect(mx, my, fuite)) {
                    if(player.speed>enemy.speed){
                        running=0;
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

        // Barre de vie joueur
        SDL_Rect player_hp = {50, 1000, player.hp * 3, 50};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player_hp);

        SDL_SetRenderDrawColor(renderer, 200, 250, 50, 255);
        SDL_RenderFillRect(renderer, &attack);

        // Barre de vie ennemi
        SDL_Rect enemy_hp = {50, 40, enemy.hp * 3, 50};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemy_hp);

        //bouton fuite
        SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
        SDL_RenderFillRect(renderer, &fuite);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("%d",state);
    return 0;
}

