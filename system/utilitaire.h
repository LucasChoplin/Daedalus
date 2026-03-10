#ifndef utilitaire_h //evite les inclusions multiples
#define utilitaire_h
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../structs.h"
#include "../def.h"
/** \file Inventaire.c
    \brief bibliothèque des fonctions générals 
    \author Patrick Leguillon
    \version 1.0
    \date 28 février 2026
*/

/** \brief fonction qui charge l'atals des boutons
    \param r pointeur vers le moteur de rendu SDL_Renderer
*/
void initAtlasMenu(SDL_Renderer * r);

SDL_Texture * getAtlasMenu(void);

void cleanupAtlasMenu(void);

SDL_Rect getTileRect2(int ID,int TAILLE_ATLAS);

/** \brief fonction pour vérifier si un fichier existe 
    \param nom chaîne de caractère qui contient le nom du fichier à vérifier
    \return renvoie 1 si le fichier existe et 0 dans le cas contraire 
*/
int FichierExiste(char nom[]);

/** \brief fonction pour charger les images danns une texture (enlève la couleur blanche de l'image aussi (255.255.255))
    \param nom nom du fichier bpm à charger 
    \param r pointeur vers le moteur de rendu 
    \param t pointeur vers le pointeur de Texture où stockée la Texture
 */
void chargerImage(char nom[],SDL_Renderer * r,SDL_Texture ** t);

/** \brief indique si un clic de souris touche une texture 
    \param e pointeur sur l'event de la souris enclenché sert à récupérer le x et y de la souris 
    \param r pointeur sur le RECT de la texture pour récupérer son x y w et h
    \return 1 si la souris touche la texture et 0 si elle ne touvhe pas 
*/
int detecterButtonClique(SDL_Event * e,SDL_Rect * r);

/** \brief fonction qui affiche un chiffre 
    \param r pointeur vers le moteur de rendu 
    \param t pointeur vers la texture de chiffre de 32px * 32 px
    \param nb chiffre à afficher compris entre 0 et 9 
    \param d pointeur vers le Rect contenant le x et y ou afficher le chiffre 
*/
void afficherChiffre(SDL_Renderer * r,SDL_Texture * t,int nb,SDL_Rect * d);

/** \brief fonction pour sauvegarder les données du jeu (actuellement sauvegarde pv max, attack, speed et quantités des deux premiers items )
    \param p pointeur vers le perso 1
    \param l pointeur vers la liste des items 
*/
void sauvegarder(Fighter p,item_t * l[]);

#endif 