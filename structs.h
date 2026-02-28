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

typedef struct {
    int hp;
    int max_hp;
    int attack;
    int speed;
} Fighter;

/** \brief structure pour les items  */
typedef struct{
    int nb;/**< nom de l'image de l'item*/
    SDL_Texture * t;/**< quantité de l'item */
    void (*f) (Fighter * p);/* pointeur vers la fonction pour utiliser un item*/ 
} item_t;