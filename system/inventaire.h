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

/** \brief fonction qui redonne 1 pv 
    \param p pointeur vers le perso concerné
*/
void soin1PV(Fighter * p);

/** \brief fonction qui redonne 5 pv 
    \param p pointeur vers le perso concerné
*/
void soin5PV(Fighter * p);

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
    \return renvoie 0 si aucun item n'a été utilisé ou 1 si un item à été utilisé 
 */
int detecterItemUtilise(SDL_Event * event, item_t * l[],Fighter*p);

/** \brief fonction qui choisi quelle items sont dropées et ajouter
    \param l liste des items 
    \param ennemi numéro de l'ennemi vaincu 
    \param t pointeur vers la où doit être stocké l'image à afficher 
    \return 1 si un item à été dropé
 */
int dropItem2(SDL_Renderer * r,int itemObtenu,item_t * l[],int ennemi,int itemdrop[]);

int dropItem(SDL_Renderer * r,item_t * l[],int itemDrop[], int e1,int e2,int e3,int e4, int e5, int e6);

/** \brief afficher items obtenus 
    \param r pointeur vers le moteur de rendu 
    \param itemObtenu nombre d'items obtenus
    \param tItemObtenu tableau des IDs des items obtenus
*/
void afficherItemObtenu(SDL_Renderer * r, int itemObtenu, int tItemObtenu[]);

#endif 