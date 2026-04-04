#ifndef inventaire_h //evite les inclusions multiples
#define inventaire_h
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utilitaire.h"
#include "../def.h"
#include "text.h"
/** \file inventaire.h
    \brief bibliothèque des fonctions en lien avec la gestion des items et de l'inventaire 
    \author Patrick Leguillon
    \version 1.0
    \date 28 février 2026
*/

/** \brief fonction qui redonne 5 pv 
    \param p pointeur vers le perso concerné
*/
void soin50PV(Fighter * p);

/** \brief fonction qui redonne 200 pv 
    \param p pointeur vers le perso concerné
*/
void soin200PV(Fighter * p);

/** \brief fonction qui affiche un nombre à coté d'une image de pièce
    \param r pointeur vers le moteur de rendu 
    \param nbPiece nombre de pièces à afficher
    \param x coordonnée x d'affichage du nombre de pièces
    \param y coordonnée y d'affichage du nombre de pièces
 */
void afficherPiece(SDL_Renderer * r,int nbPiece, int x, int y);

void afficherXp(SDL_Renderer * r,int xp, int x, int y);

/** \brief fonction qui affiche l'inventaire  
    \param r pointeur vers le moteur de rendu  
    \param l liste de pointeurs vers les item_t
    \param p pointeur vers le perso
*/
void afficherInventaire(SDL_Renderer * r, item_t * l[],Fighter * p);

/** \brief fonction qui affiche l'inventaire en combat
    \param r pointeur vers le moteur de rendu
    \param l liste de pointeurs vers les item_t
 */
void combat_afficher_inventaire(SDL_Renderer * r, item_t * l[]);

/** \brief fonction qui détecte losqu'on appuie sur un item et diminue sa quantité 
    \param event pointeur vers l'événement souris cliqué 
    \param l liste de pointeurs vers les item_t
    \param p pointeur vers le perso du joueur
    \return renvoie 0 si aucun item n'a été utilisé ou 1 si un item à été utilisé 
 */
int detecterItemUtilise(SDL_Event * event, item_t * l[],Fighter*p);

/** \brief fonction qui choisi quelle items sont dropées et ajouter
    \param l liste des items 
    \param ennemi numéro de l'ennemi vaincu 
    \param t pointeur vers la où doit être stocké l'image à afficher 
 */
void dropItem2(item_t * l[], int ennemi,Fighter * p, loot_t * d);

/** \brief fonction qui gère le loot pour un ennemi vaincu et ajoute les items correspondants, stocke les items obtenus dans itemDrop[]
    \param l liste des items 
    \param itemDrop tableau qui stocke les numéros des items obtenus, itemDrop[0] correspond au nombre d'items obtenus, itemDrop[1] correspond au nombre de pièces gagnés
    \param p pointeur vers le perso du joueur
    \param e1 numéro du premier ennemi vaincu 
*/
loot_t dropItem(item_t * l[],Fighter * p, int e1/*,int e2,int e3,int e4, int e5, int e6*/);

/** \brief afficher items obtenus 
    \param r pointeur vers le moteur de rendu 
    \param int indique le nombre d'item obtenus 
    \param int itemObtenu[] tableau qui stocke les numéros des items obtenus
    \return retourne 0 quand a fini d'affcher l'item sinon retourne 1
*/

void afficherItemObtenu(SDL_Renderer * r, loot_t * d);

void afficherMagasin(SDL_Renderer * r,Fighter * p);

void detecterAchat(SDL_Event * event, Fighter * p, item_t * l[]);

#endif 