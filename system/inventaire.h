#ifndef inventaire_h //evite les inclusions multiples
#define inventaire_h
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utilitaire.h"
#include "../def.h"
/** \file Inventaire.c
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
    \param t pointeur vers la texture des chiffres 
    \param l liste de pointeurs vers les item_t
*/
void affficherIventaire(SDL_Renderer * r, SDL_Texture * t, item_t * l[],Fighter * p);

/** \brief fonction qui détecte losqu'on appuie sur un item et diminue sa quantité 
    \param event pointeur vers l'événement souris cliqué 
    \param l liste de pointeurs vers les item_t
 */
void detecterItemUtilise(SDL_Event * event, item_t * l[],Fighter*p);

/** \brief fonction qui choisi quelle items sont dropées et ajouter
    \param l liste des items 
    \param ennemi numéro de l'ennemi vaincu 
    \param t pointeur vers la où doit être stocké l'image à afficher 
    \return 1 si un item à été dropé
 */
int dropItem(SDL_Renderer * r,int itemObtenu,item_t * l[],int ennemi,int itemdrop[]);

/** \brief afficher items obtenus 
    \param r pointeur vers le moteur de rendu 
    \param t pointeur vers la texture à afficher 
    \param time entier indiquant le nombre de fois que la texture à été affiché 
    \return retourne 0 quand a fini d'affcher l'item sinon retourne 1
*/
void afficherItemObtenu(SDL_Renderer * r, int itemObtenu, int tItemObtenu[]);

#endif 