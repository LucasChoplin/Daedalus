#ifndef structs_h //evite les inclusions multiples
#define structs_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "def.h"

/** \file structs.h
    \brief contient les structs du projets
    \author Myriam Laaqira
    \version 1.0
    \date 12 février 2026
*/

/** \brief structure pour l'affichage du jeu  */
typedef struct{
    SDL_Renderer *renderer; /**< pointeur vers le moteur de rendu */
    SDL_Window *window; /**< pointeur vers la fenêtre du jeu */
} Game;

/** \brief structure pour l'affichage du joueur  */
typedef struct{
    int xTile; /**< position x du joueur en tuile */
    int yTile; /**< position y du joueur en tuile */
    int facing; /**< direction vers laquelle le joueur fait face (DROITE, GAUCHE) */
    int spriteID; /**< ID du sprite du joueur */
    SDL_Texture *texture; /**< texture du joueur */
} Player;

/** \brief structure pour une salle de la carte */
typedef struct{
    int mapID; /**< ID de la salle dans l'étage */
    int xSalle; /**< position x de la salle dans l'étage */
    int ySalle; /**< position y de la salle dans l'étage */
    int tiles[SALLE_HEIGHT][SALLE_WIDTH]; /**< tableau des tuiles de la salle */
    SalleType type; /**< type de la salle (normale, boss, coffre, etc.) */
} Salle;

/** \brief structure pour un mob */
typedef struct{
    int mapID, spriteID, xTile, yTile, vaincu;
    int hp, max_hp, attack, speed;
} Mob;

/** \brief structure pour le personnage */
typedef struct {
    int classeID;/**< indique la classe du personnage */
    int hp; /**< points de vie actuels du personnage */
    int max_hp; /**< points de vie maximum du personnage */
    int attack; /**< points d'attaque du personnage */
    int speed; /**< vitesse du personnage */
    int xp; /**< points d'expérience du personnage */
    int max_xp; /**< points d'expérience nécessaires pour monter de niveau */
    int lvl; /**< niveau du personnage */
    int gold; /**< quantité d'or possédée par le personnage */
    int pm_atk; /**< quantité de points de magie actuels de personnage */
    int max_pm; /**< quantité maximum de points de magie du personnage */
} Fighter;

/** \brief structure pour les items  */
typedef struct{
    int nb;/**< quantité de l'item */
    void (*f) (Fighter * p);/**< pointeur vers la fonction pour utiliser un item*/ 
} item_t;

/** \brief structure pour les loots */
typedef struct{
    int nbItem; /**< nombre d'items dans le tableau item */
    int item[MAX_DROP];/**< tableau contennant le numéro des items obtenus */
    int or;/**< quantité d'or obtenu */
    int xp;/**< points d'expérience obtenus */
}loot_t;

/** \brief structure pour un bouton */
typedef struct{
    SDL_Rect rect; /**< rectangle de la zone cliquable du bouton */
    SDL_Color couleurFond; /**< couleur de fond du bouton */
    SDL_Color couleurTexte; /**< couleur du texte du bouton */
    char *texte; /**< texte affiché sur le bouton */
    TTF_Font *font; /**< police du texte du bouton */
}Bouton;

extern Game game;

#endif