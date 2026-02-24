typedef struct{
    SDL_Renderer *renderer;
    SDL_Window *window;
    int up;
    int down;
    int left;
    int right;
} Game;

typedef struct{
    int xTile, yTile, facing;
    SDL_Texture *texture;
} Player;

extern Game game;