#ifndef map_h //evite les inclusions multiples
#define map_h
#include <SDL2/SDL.h>
#include "../def.h"
#include "../structs.h"

/** \file map.h
    \brief librairie pour générer la map et déplacer le personnage 
    \author Myriam Laaqira
    \version 1.0
    \date 14 février 2026
*/

/** \brief verifie si une tile est un mur
    \param tile numero de la tile a verifier
    \return 1 si c'est un mur 0 sinon
*/
int isWall(int tile);

/** \brief verifie si une tile est un sol de combat
    \param tile numero de la tile a verifier
    \return 1 si c'est un sol de combat 0 sinon
*/
int isCombatTile(int tile);

/** \brief genere une salle en fct de son type
     \param salle pointeur vers la salle a generer
     \param type type de salle (SALLE_NORMALE, SALLE_BOSS, etc)
*/
void genererSalle(Salle* salle, SalleType type);

/** \brief initialise un etage entier 
*/
void initMap(void);

/** \brief initialise un etage en tenant compte du floor
    \param etageActuel numero d'etage courant
    \param bossRoomID id de la salle boss choisie (sortie)
    \param miniBossRoomID id de la salle miniBoss choisie (sortie, hors salle boss)
*/
void initMapParEtage(int etageActuel, int *bossRoomID, int *miniBossRoomID);

/** \brief retourne l'ID d'une salle aléatoire
    \return ID de la salle
*/
int getIDSalleRandom(void);

/** \brief retourne l'ID d'une salle active aléatoire différente de celle passée en paramètre
    \param mapIDExclu ID de salle a exclure
    \return ID de salle active (ou mapIDExclu si aucune autre salle disponible)
*/
int getIDSalleRandomExcluant(int mapIDExclu);

/** \brief force la salle courante à une salle donnée
    \param mapID ID de la salle à afficher
*/
void setCurrentMapByID(int mapID);

/** \brief dessine la map actuelle (l'etage)
    \param renderer pointeur vers le renderer
*/
void drawMap(SDL_Renderer* renderer);

/** \brief dessine un mob sur la map
    \param renderer pointeur vers le renderer
    \param mob pointeur vers le mob a dessiner
*/
void drawMob(SDL_Renderer* renderer, Mob* mob);


/** \brief dessine le joueur sur la map
    \param renderer pointeur vers le renderer
    \param player pointeur vers le player a dessiner
*/
void drawPlayer(SDL_Renderer* renderer, Player* player);

/** \brief nettoie la map actuelle
*/
void cleanupMap(void);

/** \brief change l'affichage de la salle en fct de la direction du perso
    \param direction direction du changement de salle
    \param playerX pointeur vers la position X du joueur
    \param playerY pointeur vers la position Y du joueur
*/
void changeSalle(int direction, int* playerX, int* playerY);

/** \brief transforme une salle active en salle de boss via genererSalleBoss
    \param mapID id de la salle a transformer
*/
void genererSalleBossByID(int mapID);

extern Salle* currentMap;
extern Salle* salles[];

#endif