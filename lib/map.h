#ifndef map_h //evite les inclusions multiples
#define map_h
#include <SDL2/SDL.h>
#include "def.h"
#include "structs.h"

/** \file map.h
    \brief librairie pour générer la map et déplacer le personnage 
    \author Myriam Laaqira
    \version 1.0
    \date 14 février 2026
*/

/** \brief vérifie si une tuile est un mur
    \param tile numéro de la tuile à vérifier
    \return 1 si c'est un mur 0 sinon
*/
int isWall(int tile);

/** \brief vérifie si une tuile est un sol de combat
    \param tile numéro de la tuile à vérifier
    \return 1 si c'est un sol de combat 0 sinon
*/
int isCombatTile(int tile);

/** \brief génère une salle en fonction de son type
     \param salle pointeur vers la salle à générer
     \param type type de salle (SALLE_NORMALE, SALLE_BOSS, etc)
*/
void genererSalle(Salle* salle, SalleType type);

/** \brief initialise un étage en tenant compte de l'étage
    \param etageActuel numéro de l'étage actuel (1 à 3)
    \param bossRoomID id de la salle boss choisie 
    \param miniBossRoomID id de la salle miniBoss choisie
    \param trocRoomID id de la salle troc si presente, sinon on met une salle de coffre
*/
void initMapParEtage(int etageActuel, int *bossRoomID, int *miniBossRoomID, int *trocRoomID);

/** \brief retourne l'ID d'une salle aléatoire
    \return ID de la salle
*/
int getIDSalleRandom(void);

/** \brief retourne l'ID d'une salle active aléatoire différente de celle passée en paramètre
    \param mapIDExclu ID de salle à exclure
    \return ID de salle active (ou mapIDExclu si aucune autre salle disponible)
*/
int getIDSalleRandomExcluant(int mapIDExclu);

/** \brief change la salle actuelle où le joueur se trouve
    \param mapID ID de la salle à afficher
*/
void setCurrentMapByID(int mapID);

/** \brief dessine la map actuelle (l'étage)
    \param renderer pointeur vers le moteur de rendu
*/
void drawMap(SDL_Renderer* renderer);

/** \brief dessine un mob sur la map (miniboss, boss ou marchand)
    \param renderer pointeur vers le moteur de rendu
    \param mob pointeur vers le mob à dessiner
*/
void drawMob(SDL_Renderer* renderer, Mob* mob);


/** \brief dessine le joueur sur la map
    \param renderer pointeur vers le moteur de rendu
    \param player pointeur vers le joueur à dessiner
*/
void drawPlayer(SDL_Renderer* renderer, Player* player);

/** \brief nettoie la map actuelle
*/
void cleanupMap(void);

/** \brief change l'affichage de la salle en fonction de la direction du joueur
    \param direction direction du changement de salle
    \param playerX pointeur vers la position X du joueur
    \param playerY pointeur vers la position Y du joueur
*/
void changeSalle(int direction, int* playerX, int* playerY);

extern Salle* currentMap;
extern Salle* salles[];

#endif