#ifndef DEF_H
#define DEF_H

//dimension fenetre
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960

//dimension de l'etage entier
#define ETAGE_WIDTH 3
#define ETAGE_HEIGHT 3

//dimension d'une salle
#define SALLE_WIDTH 20
#define SALLE_HEIGHT 15

//dimension d'une tile
#define TILE_SIZE 64

//taille des atlas 
#define ATLAS_MAP 7
#define ATLAS_BOUTON 4
#define ATLAS_ITEM 2
#define ATLAS_PERSO 2

//direction
enum Direction {DROITE, BAS, GAUCHE, HAUT};

//dimension des sprites 
#define TAILLE_CHIFFRE 32 //taille des chiffres 32*32px
#define TAILLE_ITEM 32//taille donnée au item
#define TAILLE_SPRITE 128//taille donné au sprite  
#define TAILLE_MENU 128//taille des menus 
//items
#define NB_ITEM 2//nombre d'items dans le jeu 

//fichier de sauvegarde
#define FICHIER_DATA "data.txt"//nom du fichier où sont sauvegardé les données 

//stats des classes
#define ARCHER 0
#define GLADIATEUR 1
#define LANCIER 2
#define GLADIATEUR_MAX_HP 200
#define GLADIATEUR_ATTACK 120
#define GLADIATEUR_SPEED 100
#define ARCHER_MAX_HP 100
#define ARCHER_ATTACK 220
#define ARCHER_SPEED 100
#define LANCIER_MAX_HP 100
#define LANCIER_ATTACK 120
#define LANCIER_SPEED 200


typedef enum {
    DEFAITE,
    VICTOIRE,
    ENNEMY,
    PLAYER,
} GameState;


#endif
